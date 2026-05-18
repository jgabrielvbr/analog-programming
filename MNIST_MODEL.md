# MNIST via Analog Signal Framework — Math & Code

This document traces the full pipeline from raw pixel data to a trained digit
classifier, showing how each mathematical idea is expressed in the C++ signal
framework.

---

## 1. The Signal Primitives

### Wave

A `wave` is a single complex exponential:

```
wave(ω, A, φ)  →  A · e^{j(ωx + φ)}
```

where `ω` is frequency, `A` is amplitude, `φ` is phase, and `x` is the
evaluation point.

```cpp
// wave.cpp
std::complex<double> wave::evaluate(double x) const
{
    const std::complex<double> j{0.0, 1.0};
    return amplitude_ * std::exp(j * (frequency_ * x + phase_));
}
```

### Function

A `function` is a superposition (sum) of waves — a finite discrete spectrum:

```
f(x) = Σ_k  A_k · e^{j(ω_k · x + φ_k)}
```

Waves are stored **sorted by frequency** (via `std::upper_bound` insertion),
which makes frequency lookups (`get_wave`) an O(log n) binary search.

```cpp
// function.cpp — evaluate
std::complex<double> function::evaluate(double x) const
{
    std::complex<double> result{0.0, 0.0};
    for (const auto& wave : waves_)
        result += wave.evaluate(x);
    return result;
}
```

---

## 2. Input Encoding — Pixels as a Spectrum

An MNIST image is 28 × 28 = **784 grayscale pixels**, each an integer in
[0, 255]. We encode it as an `analog::function` with 784 waves:

```
pixel index i  →  frequency  ω = i
pixel value  v  →  amplitude  A = v / 255.0
phase             →  φ = 0
```

So image pixel `p[i]` becomes the wave:

```
p[i] / 255.0 · e^{j · i · x}
```

The image is now a **discrete spectrum**: each frequency slot holds the
intensity of the corresponding pixel.

```cpp
// mnist_loader.cpp
for (uint32_t i = 0; i < n_pixels; ++i)
    image.add_wave(analog::wave{
        static_cast<double>(i),   // ω = pixel index
        buf[i] / 255.0,           // A = normalised intensity
        0.0                       // φ = 0  (purely real encoding)
    });
```

Because all phases are zero, `A = amplitude()` is the only information-bearing
part. The complex exponential machinery is available but not exercised here —
that is intentional: the framework stays general while this use-case stays real.

---

## 3. Architecture Overview

```
image (analog::function, 784 waves)
    │
    │  spectral weighting  ×  W  (element-wise per frequency)
    ▼
pre-activation  z[i] = x[i] · w[i]
    │
    │  ReLU
    ▼
hidden activations  h[i] = max(0, z[i])
    │
    │  linear readout  ×  R  (10 × 784 weight matrix)
    ▼
logits  s[k] = Σ_i  h[i] · R[k][i]
    │
    │  softmax
    ▼
probabilities  p[k]
    │
    │  cross-entropy  vs.  one-hot label
    ▼
loss  L
```

Two weight tensors are learned:
- **W** — a vector of 784 scalars (one per frequency / pixel), held in `layer`
- **R** — a 10 × 784 matrix (one row per digit class), held in `readout`

---

## 4. Hidden Layer — Spectral Weighting

### Forward pass

For each frequency index `i`, the layer multiplies the input amplitude by a
learnable scalar weight:

```
z[i] = x[i] · w[i]       (pre-activation)
h[i] = max(0, z[i])      (ReLU activation)
```

This is the **spectral convolution** of the input function with the weight
function evaluated at matching frequencies. Formally, if:

```
X  =  Σ_i  x[i] · e^{j·i·t}      (input)
W  =  Σ_i  w[i] · e^{j·i·t}      (weights as a function)
```

then `(X ∗ W)[i] = x[i] · w[i]` — convolution of two same-frequency-set
functions is elementwise amplitude multiplication. The framework's
`convolution::apply` implements exactly this.

The layer uses raw `vector<double>` instead of calling `convolution::apply`
so that `z[i]` and `x[i]` can be cached for the backward pass without
constructing intermediate `analog::function` objects.

```cpp
// layer.cpp — forward
for (int i = 0; i < n_inputs_; ++i)
{
    const analog::wave* w = input.get_wave(static_cast<double>(i));
    const double x = w ? w->amplitude() : 0.0;

    cached_x_[i] = x;
    pre_relu_[i] = x * weights_[i];      // z[i]
    h[i]         = std::max(0.0, pre_relu_[i]);   // h[i]
}
```

You can verify equivalence to the signal framework at any time:

```cpp
// These two should produce identical pre-ReLU amplitudes:
auto h_direct    = layer.forward(image);                        // raw path
auto h_framework = convolution::apply(image, layer.weights_as_function()); // signal path
```

### Weight initialisation

Weights are drawn from Uniform(−ε, +ε) with ε = 0.01:

```cpp
std::uniform_real_distribution<double> dist(-init_scale, init_scale);
for (auto& w : weights_) w = dist(rng);
```

Small initialisation keeps pre-activations near zero at the start, preventing
saturated (always-positive or always-zero) ReLU outputs before the first update.

---

## 5. Readout — Linear Classification

The readout maps the 784-dimensional hidden activation to 10 class scores.
Each class `k` has its own weight vector `R[k]` of length 784. The score for
class `k` is the dot product:

```
s[k] = Σ_i  h[i] · R[k][i]
```

Equivalently, thinking in signal terms: `s[k]` is the total amplitude of
`convolution::apply(h_function, R_k_function)`, where `R_k_function` is class
`k`'s weights expressed as an `analog::function`. The sum of amplitudes of the
spectral product is the inner product in amplitude space.

```cpp
// readout.cpp — forward
for (int k = 0; k < n_classes_; ++k)
    for (int i = 0; i < n_inputs_; ++i)
        scores[k] += h[i] * R_[k][i];
```

---

## 6. Loss — Softmax Cross-Entropy

### Softmax

Converts raw scores into a probability distribution:

```
p[k] = exp(s[k] − max_s) / Σ_j exp(s[j] − max_s)
```

Subtracting `max_s` before exponentiating is a standard numerical stability
trick — it prevents overflow without changing the result.

```cpp
// network.cpp — softmax
const double max_score = *std::max_element(scores.begin(), scores.end());
for (std::size_t k = 0; k < scores.size(); ++k) {
    probs[k] = std::exp(scores[k] - max_score);
    sum += probs[k];
}
for (auto& p : probs) p /= sum;
```

### Cross-entropy

Measures how surprised the model is by the true label `y`:

```
L = −log p[y]
```

The `+ 1e-12` guards against `log(0)` if a probability collapses numerically:

```cpp
return -std::log(probs[label] + 1e-12);
```

A random model over 10 classes gives `p[y] ≈ 0.1`, so the baseline loss is
`−log(0.1) = ln(10) ≈ 2.303`. After one epoch the loss is already around 0.75
— a clear sign the gradients are working.

---

## 7. Backpropagation

Gradients flow backwards through three stages: softmax+loss → readout → layer.

### Stage 1 — Gradient through softmax + cross-entropy

The combined gradient of the cross-entropy loss with respect to the logits has
the clean closed form:

```
∂L/∂s[k] = p[k] − 1{k = y}
```

where `1{k = y}` is 1 for the true class, 0 otherwise. This vector is called
`delta`:

```cpp
// network.cpp — compute_delta
std::vector<double> delta = probs;
delta[label] -= 1.0;
```

### Stage 2 — Gradient through the readout

Given `delta[k] = ∂L/∂s[k]`:

```
∂L/∂R[k][i]  =  delta[k] · h[i]        (weight gradient)
∂L/∂h[i]     =  Σ_k  delta[k] · R[k][i] (activation gradient)
```

The activation gradient `∂L/∂h` is computed **before** updating `R`, so the
gradient uses the weights that were active during the forward pass:

```cpp
// readout.cpp — backward
// 1. compute grad_h with current R
for (int i = 0; i < n_inputs_; ++i)
    for (int k = 0; k < n_classes_; ++k)
        grad_h[i] += delta[k] * R_[k][i];

// 2. then update R
for (int k = 0; k < n_classes_; ++k)
    for (int i = 0; i < n_inputs_; ++i)
        R_[k][i] -= learning_rate * delta[k] * cached_h_[i];
```

### Stage 3 — Gradient through the layer (ReLU + spectral weight)

The ReLU gate:
```
∂h[i]/∂z[i] = 1  if z[i] > 0,  else 0
```

Chained with `∂L/∂h[i]` from Stage 2:

```
∂L/∂z[i]  =  ∂L/∂h[i]  ×  1{z[i] > 0}
```

Then through the weight multiplication `z[i] = x[i] · w[i]`:

```
∂L/∂w[i]  =  ∂L/∂z[i] · x[i]      (weight gradient)
∂L/∂x[i]  =  ∂L/∂z[i] · w[i]      (input gradient, not used here)
```

```cpp
// layer.cpp — backward
for (int i = 0; i < n_inputs_; ++i)
{
    const double relu_mask = pre_relu_[i] > 0.0 ? 1.0 : 0.0;
    const double grad_pre  = grad_h[i] * relu_mask;   // ∂L/∂z[i]

    grad_x[i]    = grad_pre * weights_[i];             // ∂L/∂x[i]
    weights_[i] -= learning_rate * grad_pre * cached_x_[i]; // SGD step on w[i]
}
```

---

## 8. Weight Update — Stochastic Gradient Descent

After each sample, weights are updated in the direction that decreases loss:

```
w[i]    ←  w[i]    − η · ∂L/∂w[i]
R[k][i] ←  R[k][i] − η · ∂L/∂R[k][i]
```

with learning rate η = 0.01. This is **online SGD** (one sample at a time),
which converges faster in practice than batch gradient descent on MNIST.

The training loop shuffles the dataset each epoch with a fixed seed (42) for
reproducibility, then iterates through all 60 000 samples:

```cpp
// network.cpp — train
std::mt19937 rng{42};
for (int epoch = 0; epoch < epochs; ++epoch)
{
    std::shuffle(indices.begin(), indices.end(), rng);
    for (int idx : indices)
        train_one(train_data.images[idx], train_data.labels[idx], learning_rate);
}
```

---

## 9. Observed Results

| Epoch | Avg Loss | Train Acc | Test Acc |
|------:|:--------:|:---------:|:--------:|
| 1     | 0.748    | 81.6%     | 87.4%    |
| 2     | 0.417    | 90.6%     | 89.2%    |
| 5     | 0.368    | 91.7%     | 90.2%    |
| 10    | 0.347    | 92.0%     | **90.7%**|

The test accuracy exceeding train accuracy in the early epochs is expected: the
training accuracy is measured *after* each sample's weight update (so early
samples in an epoch were evaluated with older weights), while test accuracy is
measured once with the final weights.

---

## 10. Limitations and Directions to Explore

| Limitation | Possible change |
|---|---|
| **Single hidden layer** | Stack two or more `layer` objects; pass `grad_x` from each backward into the next layer's backward |
| **No spatial structure** | The pixel-index-as-frequency encoding treats the image as a flat sequence. A 2D Fourier encoding (row freq + column freq as two separate wave components) could preserve spatial locality |
| **Linear spectral weighting** | The hidden layer is a diagonal linear map. Replacing it with a preset (e.g. `gaussian` or `resonance`) as the weight function makes the spectral shaping interpretable as a physical filter |
| **Slow training** | Each sample requires two forward passes (train + accuracy check). Moving the accuracy measurement to end-of-epoch reduces compute by ~40% |
| **Fixed learning rate** | Adding momentum or an adaptive rule (Adagrad: `lr / sqrt(sum_of_squares_of_past_grads)`) typically helps convergence on MNIST |
| **ReLU dead neurons** | With small random init and many zero pixels, many `w[i]` may land negative and stay dead. Leaky ReLU (`max(α·z, z)` with α = 0.01) keeps gradients alive |
