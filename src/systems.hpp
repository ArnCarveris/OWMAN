#pragma once

struct Systems
{
    template<typename Archive>
    void load(Archive& ar);

    template<typename Archive>
    void save(Archive& ar) const;
};
