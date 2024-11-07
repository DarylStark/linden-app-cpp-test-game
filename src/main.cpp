#include <linden_utils/frame_rate_limiter.h>
#include <linden_utils/scoped_timer.h>

int main()
{
    while (true)
    {
        linden::utils::ScopedTimer scopedTimer;
        linden::utils::FrameRateLimiter frameRateLimiter(60);
    }
    return 0;
}