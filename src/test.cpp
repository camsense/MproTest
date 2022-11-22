#include "Mpro.h"
#include <thread>
#include <iostream>
#include <chrono>

uint64_t getCurrentTimestampNs()
{
    auto ts = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return ts;
}


int main(int argc,  char ** argv)
{
    Mpro app;
    app.Init();
    app.Start();
    bool isExit = false;
    bool bFirst = true;
    uint64_t u64SysTs1 = 0;
    uint64_t u64MproTsMs1 = 0;

    std::thread thr([&] {
        
        while (!isExit)
        {
            std::list<HPP::RigidInfo> lis;
            app.GetResult(lis);
            if (lis.empty())
            {
                continue;
            }
            if (bFirst)
            {
                bFirst = false;
                u64SysTs1 = getCurrentTimestampNs();

                auto iter = lis.begin();
                u64MproTsMs1 = iter->time_stamp;
            }

            for (auto iter = lis.begin(); iter != lis.end(); ++iter) 
            {
                uint64_t u64SysTsN = u64SysTs1 + (iter->time_stamp - u64MproTsMs1)*1000;
                printf("pose: state-%d, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %lld, %lld\n",
                    iter->states, 
                    iter->pose.quaternion[0],
                    iter->pose.quaternion[1],
                    iter->pose.quaternion[2],
                    iter->pose.quaternion[3],
                    iter->pose.translation[0],
                    iter->pose.translation[1],
                    iter->pose.translation[2],
                    iter->time_stamp,
                    u64SysTsN
                );
            }
            //std::this_thread::sleep_for(std::chrono::microseconds(1));
            //printf("list size:%zd\n", lis.size());
        }
        });
    std::thread thr1([&] {
        getchar();
        isExit = true;
        });

    printf("wait............\n");
    thr.join();
    thr1.join();
    app.Deinit();

    return 0;
}


