#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <functional>
#include <ctime>

struct progress_t
{
    std::atomic<std::thread::id> id {} ;
    std::atomic<unsigned int> how_much {} ;
    std::atomic<std::time_t> when {} ;
};

void thread_fun( progress_t& progress )
{
    progress.id = std::this_thread::get_id() ;
    progress.how_much = 0 ;

    for( int i = 0 ; i < 4 ; ++i )
    {
        // simulate progress by sleeping for one second
        std::this_thread::sleep_for( std::chrono::seconds(1) ) ;
        progress.how_much += 25 ;
        progress.when = std::time(nullptr) ;
    }
}

int main()
{
    constexpr std::size_t NTHREADS = 1 ;
    progress_t progress[NTHREADS] ;
    std::vector< std::future<void> > futures ;

    for( std::size_t i = 0 ; i < NTHREADS ; ++i )
        futures.push_back( std::async( std::launch::async, thread_fun, std::ref( progress[i] ) ) ) ;

    unsigned int total_progress = 0 ;
    while( total_progress < 100 * NTHREADS )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(1100) ) ;

        std::string out_str = "\r";
        total_progress = 0 ;
        for( const auto& p : progress )
        {
            total_progress += p.how_much ;
            out_str += "thread #" +  std::to_string(p.how_much) + "%\n";
            // std::time_t t = p.when ;
            // std::cout << std::ctime( std::addressof(t) );
        }

        std::clog << out_str << std::flush << std::flush;
    }

    std::cout << "all threads reported 100% progress\n" ;
}