#include <iostream>
#include <csignal>

///////// Backward tracing begin /////////
#define BACKWARD_HAS_DW 1
#include <backward.hpp>
backward::SignalHandling sh; static void trace()
{ backward::StackTrace st; st.load_here(32); backward::Printer p; p.print(st); }
///////// Backward tracing end /////////

#if _WIN32
#   include <conio.h>
#   define kbhit _kbhit
#   define millsleep(n) Sleep(n)
#else
#   include <sys/ioctl.h>
#   include <termios.h>
#   define millsleep(n) usleep(n * 1000)
bool kbhit()
{
    termios term; tcgetattr(0, &term);
    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting = 0;
    ioctl(0, FIONREAD, &byteswaiting);
    tcsetattr(0, TCSANOW, &term);
    return byteswaiting > 0;
}
#endif

void signalHandler(int signum)
{ 
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    // 清理并关闭  
    exit(signum);  
}

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);
    try
    {
        std::cout << "Hello World!"  << std::endl;
    }
    catch (std::exception const & e)
    { std::cout << e.what() << std::endl; }
    catch (...)
    { std::cout << "other exception" << std::endl; }
    return 0;
}
