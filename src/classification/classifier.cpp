#include "classifier.hpp"

#include "interaction.hpp"

#include <limits>

namespace analog
{
    void classifier::add_prototype(const prototype &prototype)
    {
        prototypes_.push_back(prototype);
    }

    int classifier::classify(const function &input) const
    {
        double best_score = std::numeric_limits<double>::max();

        int best_label = -1;

        for (const auto &prototype : prototypes_)
        {
            const double score = interaction::difference(input, prototype.representation(), 0.0, 10.0, 512);

            if (score < best_score)
            {
                best_score = score;
                best_label = prototype.label();
            }
        }

        return best_label;
    }
}