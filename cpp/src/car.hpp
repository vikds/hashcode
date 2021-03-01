#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Car {
public:
    Car(const std::vector<Street*>& path);

    Street* current_street();

    size_t finish_time() const {
        return finish_time_;
    }
    size_t left_to_go() const {
        return left_to_go_;
    }
    size_t min_required_time() const {
        return min_required_time_;
    }
    std::vector<Street*> path() const {
        return path_;
    }

public:
    void Tick(size_t time);
    void Reset();

    bool HasFinished() const;
    bool IsWaiting() const;

private:
    void Turn(size_t time);

private:
    std::vector<Street*> path_;
    size_t min_required_time_;

private: // resetable
    size_t position_;
    size_t finish_time_;
    size_t left_to_go_;
};

} // namespace hashcode