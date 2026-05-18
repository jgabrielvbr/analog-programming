# Resonator Network for MNIST

A digit classifier built entirely from analog signal-processing primitives: waves,
superpositions, and Lorentzian resonators. The model achieves **~90.6% test accuracy**
on MNIST with only ~900 learnable parameters and no activation functions.

---

## 1. Motivation

The goal is a machine-learning architecture that can be directly realized as an
**analog electric circuit**: no matrix multiplications, no nonlinearities bolted on
after the fact, just physically meaningful operations — resonance, energy detection,
superposition.

A Lorentzian resonator in a circuit (an LC filter) responds selectively to a narrow
band of frequencies. Its output amplitude is proportional to the energy of the input
signal in that band. This is the single primitive the hidden layer is built from.

---

## 2. Core Data Structures

### `analog::wave`

A single complex oscillator:

```
wave(ω, A, φ)  →  A · e^{j(ωx + φ)}
```

Stored as three scalars: `frequency` ω, `amplitude` A, `phase` φ.

### `analog::function`

A superposition (sum) of waves — a discrete Fourier-like representation:

```
F(x) = Σ_k  A_k · e^{j(ω_k · x + φ_k)}
```

Internally a sorted `vector<wave>`. The key operation used by the network is
`waves()`, which exposes all constituent waves so their (ω, A) pairs can be
iterated.

---

## 3. Input Encoding

Each MNIST image is 28×28 = 784 pixels. The image is encoded as an
`analog::function` with 784 waves — one per pixel:

```
pixel at position i  →  wave{ ω = i / 783,  A = pixel_value / 255,  φ = 0 }
```

**Frequency = spatial address.** Every pixel gets a unique frequency in [0, 1],
derived from its linear index `i = row * 28 + col`. Amplitude carries brightness.

This encoding preserves the full spatial layout of the image: knowing ω, you can
recover `(row, col)` exactly:

```
i   = round(ω * 783)
row = i / 28
col = i % 28
```

Why not use brightness as frequency? Because two pixels at different positions but
the same brightness would produce the same frequency — indistinguishable to any
filter. Position-as-frequency makes every pixel uniquely addressable.

---

## 4. The Resonator Node

### Lorentzian filter

A 1D resonator node is characterized by a **center frequency** c and **bandwidth** b.
Its frequency response is a Lorentzian (same shape as an ideal LC bandpass filter):

```
H(ω; c, b) = 1 / (1 + ((ω − c) / b)²)
```

Properties:
- Peak response H = 1 at ω = c (on resonance).
- Half-power bandwidth: the response drops to 0.5 at ω = c ± b.
- Falls off as 1/D² far from resonance (D = (ω − c) / b).
- Always in (0, 1]; no negative values.

### Inner product (the node's output)

Rather than measuring energy (∑ A² · H²), the output is the **inner product** of
the input function with the filter response:

```
out = ⟨F, H⟩ = Σ_j  A_j · H(ω_j;  c, b)
```

This is a weighted sum of pixel amplitudes, where the weight for pixel j is its
Lorentzian response at ω_j. Pixels close to c in frequency space get weight ≈ 1;
distant pixels get weight ≈ 0.

Because position IS frequency in our encoding, a 1D resonator with center c and
bandwidth b effectively computes the **weighted average brightness of a horizontal
strip** of the image.

### 2D separable resonator

A single horizontal strip is not enough — MNIST needs to detect rectangular patches
("is the top-left corner bright?"). The 2D node uses a **separable product** of two
independent Lorentzians:

```
H(r, c; c_r, b_r, c_c, b_c) = H_r(r/27; c_r, b_r) · H_c(c/27; c_c, b_c)
```

where r = row index ∈ [0, 27], c = column index ∈ [0, 27], both normalized to [0, 1].

The node's output:

```
out = Σ_{pixels j}  A_j · H_r(r_j / 27) · H_c(c_j / 27)
```

This is the **weighted average brightness of a rectangular patch**, where the patch
shape is determined by (c_r, b_r) in the row direction and (c_c, b_c) in the column
direction.

Learnable parameters per node: `(c_r, b_r, c_c, b_c)` — four scalars.

---

## 5. Architecture

```
raw image  (28×28 pixels)
    │
    │  encoding: freq = i/783,  amp = pixel/255
    ▼
F_in  =  Σ_{i=0..783}  (pixel_i/255) · e^{j·(i/783)·x}
    │
    │  Hidden layer: 64 resonator_node_2d in an 8×8 grid
    │  node k: out_k = Σ_j  A_j · H_r(r_j/27) · H_c(c_j/27)
    │
    ▼
h  =  [out_0, out_1, ..., out_63]   ← 64 spatial patch scores
    │
    │  Readout: 10×64 signed weight matrix R
    │  logit_k = Σ_i  h[i] · R[k][i]
    │
    ▼
logits  =  [logit_0, ..., logit_9]
    │
    │  softmax → cross-entropy vs. one-hot label
    ▼
loss L
```

### Hidden layer — 8×8 grid of patch detectors

64 nodes are arranged in an 8×8 spatial grid. Node (ir, ic) is initialized at:

```
c_r = (ir + 0.5) / 8,    b_r = 1/8 = 0.125
c_c = (ic + 0.5) / 8,    b_c = 1/8 = 0.125
```

This places nodes evenly across the image at initialization. Each node's receptive
field covers roughly a 3.5×3.5 pixel patch (one eighth of the 28-pixel side).

Parameters (c_r, b_r, c_c, b_c) are learned by gradient descent and can drift from
the initial grid positions.

### Output layer — signed linear readout

A plain 10×64 weight matrix R, initialized from Uniform(−0.01, 0.01).

The inner product outputs h[k] are always ≥ 0 (all amplitudes and Lorentzian
values are non-negative). A single-valued feature cannot represent "class A needs
region X bright AND region Y dark." The signed readout solves this: a negative
weight R[k][i] contributes negatively when patch i is bright, allowing patterns like
"top patches positive, bottom patches negative" for the digit '1' vs '8'.

Total parameters: 64×4 (resonator) + 10×64 (readout) = **896 learnable scalars**.

---

## 6. Forward Pass

### Step 1 — Hidden layer

```cpp
// resonator_node_2d::forward
for each pixel j:
    idx = round(ω_j * 783)         // recover linear index
    r   = idx / 28                 // row ∈ [0, 27]
    c   = idx % 28                 // col ∈ [0, 27]
    Dr  = (r/27.0 - c_r) / b_r
    Dc  = (c/27.0 - c_c) / b_c
    Hr  = 1 / (1 + Dr²)
    Hc  = 1 / (1 + Dc²)
    out += A_j * Hr * Hc           // inner product
```

Cache (A_j, H_r_j, H_c_j, row_j, col_j) for the backward pass.

### Step 2 — Readout

```
logit[k] = Σ_i  h[i] · R[k][i]
```

### Step 3 — Softmax + cross-entropy

```
probs[k] = exp(logit[k] − max) / Σ exp(logit[k'] − max)
loss      = −log(probs[label])
```

---

## 7. Backward Pass

### Softmax gradient

The gradient of cross-entropy loss with respect to logits — the **delta vector**:

```
δ[k] = probs[k] − 1_{k == label}
```

δ[correct] ≈ −0.9 (pulling logit up); δ[wrong] ≈ +0.1 (pushing logit down).

### Readout backward

```
∂L/∂h[i] = Σ_k  δ[k] · R[k][i]         ← gradient to hidden layer
∂L/∂R[k][i] = δ[k] · h[i]              ← parameter gradient

update: R[k][i] -= lr · δ[k] · h[i]
```

### Resonator node backward

Each node k receives `grad_out = ∂L/∂out_k` from the readout.

**Gradient w.r.t. input amplitudes** (passed upstream but not used — pixels are fixed):

```
∂L/∂A_j = grad_out · H_r_j · H_c_j
```

**Gradients w.r.t. node parameters**, derived from the Lorentzian:

```
∂H_r/∂c_r = 2·D_r·H_r²/b_r
∂H_r/∂b_r = 2·D_r²·H_r²/b_r
∂H_c/∂c_c = 2·D_c·H_c²/b_c
∂H_c/∂b_c = 2·D_c²·H_c²/b_c
```

Accumulated over all pixels:

```
∂L/∂c_r = Σ_j  grad_out · A_j · H_c_j · (2·D_rj·H_rj²/b_r)
∂L/∂b_r = Σ_j  grad_out · A_j · H_c_j · (2·D_rj²·H_rj²/b_r)
∂L/∂c_c = Σ_j  grad_out · A_j · H_rj · (2·D_cj·H_cj²/b_c)
∂L/∂b_c = Σ_j  grad_out · A_j · H_rj · (2·D_cj²·H_cj²/b_c)
```

---

## 8. The Critical Fix — b² Gradient Scaling

### The problem

Both ∂H/∂c and ∂H/∂b contain a **1/b factor**:

```
∂H/∂c = 2·D·H²/b    →    Δc ∝ lr/b
∂H/∂b = 2·D²·H²/b   →    Δb ∝ lr/b
```

For a node with bandwidth b = 0.125 and lr = 0.01, each update step is
**8× larger** than the equivalent step for b = 1.0. For narrow nodes (b = 0.01),
the factor is 100×. Without correction:

- Center: teleports by many bandwidths in a single sample → lands on boundary [0,1] → H ≈ 0 everywhere → node becomes deaf.
- Bandwidth: oscillates by 50%+ of its value per sample → bounces off the clamp floor → never stabilizes.

The symptom was training stuck at ~18% accuracy with test accuracy swinging
between 10% and 35% epoch to epoch.

### The fix — b² scaling

Multiply all parameter gradients by b² before applying:

```
c_r -= lr · b_r² · ∂L/∂c_r
b_r -= lr · b_r² · ∂L/∂b_r
c_c -= lr · b_c² · ∂L/∂c_c
b_c -= lr · b_c² · ∂L/∂b_c
```

Substituting:

```
Δc_r ∝ lr · b_r² · (1/b_r) = lr · b_r      ← step proportional to bandwidth
Δb_r ∝ lr · b_r² · (1/b_r) = lr · b_r      ← step proportional to bandwidth
```

The 1/b singularity cancels. A narrow node (small b) takes small steps; a wide node
takes larger steps. This is the natural scale for Lorentzian parameters — similar to
how you'd reparameterize a Gaussian in terms of σ and step in σ-space.

Effect: epoch 1 accuracy jumped from 18% → **85.8%** with no other change.

---

## 9. Results

Training: 60 000 samples, lr = 0.01, seed = 42. No momentum, no normalization.

| Epoch | Loss   | Train  | Test   |
|-------|--------|--------|--------|
| 1     | 0.511  | 85.8%  | 88.1%  |
| 2     | 0.400  | 88.3%  | 90.2%  |
| 5     | 0.363  | 89.4%  | 89.8%  |
| 8     | 0.352  | 89.9%  | 90.6%  |
| 10    | 0.348  | 90.1%  | **90.65%** |

Loss decreases monotonically. Test accuracy is stable and close to train accuracy
(minimal overfitting), as expected for a 896-parameter model on 60 K samples.

---

## 10. File Structure

```
src/
├── core/
│   ├── wave.hpp / .cpp          — single oscillator A·e^{j(ωx+φ)}
│   └── function.hpp / .cpp      — sorted superposition of waves
│
├── network/
│   ├── mnist_loader.hpp / .cpp  — IDX binary parser; encodes pixels as waves
│   ├── resonator_node_2d.hpp / .cpp  — single 2D Lorentzian patch detector
│   ├── resonator_layer_2d.hpp / .cpp — 8×8 grid of resonator_node_2d
│   ├── readout.hpp / .cpp       — 10×64 signed linear readout
│   ├── network.hpp / .cpp       — wires hidden + output, softmax, training loop
│   ├── resonator_node.hpp / .cpp — 1D resonator (kept; not used in this model)
│   └── resonator_layer.hpp / .cpp — 1D resonator layer (kept; not used)
│
├── mnist_main.cpp               — entry point; constructs network{64, 10, 0.01}
│
├── transforms/                  — amplitude/phase/frequency transforms
├── interactions/                — superposition, convolution, modulation, etc.
└── presets/                     — Gaussian, low/high/band-pass, resonance, etc.
```

### Key interfaces

**`resonator_node_2d`**
```cpp
resonator_node_2d(double c_r, double b_r, double c_c, double b_c, int n_inputs);
double              forward(const analog::function& input);   // returns ⟨F, H⟩
std::vector<double> backward(double grad_out, double lr);    // updates params, returns ∂L/∂A_j
```

**`resonator_layer_2d`**
```cpp
resonator_layer_2d(int n_nodes, int n_inputs, int n_r, int n_c, double bw_r, double bw_c);
analog::function    forward(const analog::function& input);         // → one score per node
std::vector<double> backward(const std::vector<double>& delta, double lr);
```

**`readout`**
```cpp
readout(int n_classes, int n_inputs, double init_scale);
std::vector<double> forward(const std::vector<double>& h);          // → 10 logits
std::vector<double> backward(const std::vector<double>& delta, double lr); // → ∂L/∂h
```

**`network`**
```cpp
network(int n_hidden = 64, int n_classes = 10, double init_scale = 0.01);
void train(const mnist_dataset&, const mnist_dataset&, int epochs, double lr);
```

---

## 11. Physical Interpretation

Each hidden node is a **tuned LC bandpass filter** followed by an integrator:

1. **Filter**: The input signal F(x) = Σ A_j · e^{jω_j x} passes through H(ω).
   Frequencies near resonance (ω ≈ c) pass with gain ≈ 1; far-off frequencies
   are attenuated as 1/(ω−c)².

2. **Detector**: The output is the DC component of F(x) · H∗(x) — equivalently,
   the inner product ⟨F, H⟩. Physically, this is what a synchronous detector
   (lock-in amplifier) computes.

3. **2D extension**: Two orthogonal filters (one for each spatial axis) in series,
   their outputs multiplied. Implementable as two cascaded LC stages.

The readout is a signed summing amplifier — a standard op-amp weighted adder.
The whole network is realizable with ~70 tunable analog components.

The **parameters** (c, b) map directly to circuit components:
- Center frequency c → the LC resonant frequency f₀ = 1/(2π√LC)
- Bandwidth b → the quality factor Q = f₀ / BW

Learning the network means **tuning the physical circuit**.
