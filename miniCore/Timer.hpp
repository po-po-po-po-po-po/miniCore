#pragma once

struct Timer
{
    void reset() { startTime = millis(); }
    int64_t getTime() { return millis() - startTime; }

private:
    uint64_t startTime;
};
