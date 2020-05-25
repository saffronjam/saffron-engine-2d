#include "App.h"

class InitException;

int main(int, char **)
{
    try
    {
        App app;
        app.Run();
    }
    catch (const VeException &e)
    {
        log_fatal("Type: %s What: %s", e.GetType(), e.what());
    }
    catch (const std::exception &e)
    {
        log_fatal("Type: %s What: %s", "Standard Exception", e.what());
    }
    catch (...)
    {
        log_fatal("Type: %s What: %s", "Standard Exception", "No details available");
    }
    return EXIT_SUCCESS;
}
