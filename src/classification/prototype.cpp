#include "prototype.hpp"

namespace analog
{
    prototype::prototype(int label, function representation) : label_(label), representation_(std::move(representation))
    {
    }

    int prototype::label() const
    {
        return label_;
    }

    const function &prototype::representation() const
    {
        return representation_;
    }
}