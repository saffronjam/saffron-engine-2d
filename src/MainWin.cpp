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
        Log_fatal("Type: ", e.GetType(), "What: ", e.what());
    }
    catch (const std::exception &e)
    {
        Log_fatal("Type: ", "Standard Exception", "What: ", e.what());
    }
    catch (...)
    {
        Log_fatal("Type: ", "Standard Exception", "What: ", "No details available");
    }
    return EXIT_SUCCESS;
}
