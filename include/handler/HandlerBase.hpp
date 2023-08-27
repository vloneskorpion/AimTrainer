#pragma once

class HandlerBase
{
    public:
        HandlerBase() = default;
        virtual ~HandlerBase() = default;

        virtual void handle() = 0;
};