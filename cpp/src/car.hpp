#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Car {
public:
    Car(const std::vector<Street*>& path);

    size_t finish_time() const {
        return finish_time_;
    }
    size_t left_to_go() const {
        return left_to_go_;
    }
    std::vector<Street*>& path() {
        return path_;
    }

public:
    void Reset();
    void Tick(size_t time);
    bool HasFinished() const;
    bool IsWaiting() const;

private:
    void Turn(size_t time);

private:
    std::vector<Street*> path_;

private: // resetable
    size_t position_;
    size_t finish_time_;
    size_t left_to_go_;
};

} // namespace hashcode