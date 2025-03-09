//#define CROW_USE_BOOST 1

#define CROW_STATIC_DIRECTORY "C:\Users\Nazar\source\repos\Bridge-Online\web\static"

#include "crow.h"
#include "../../core/core.hpp"


constexpr const char* const TemplatesDir = "C:\\Users\\Nazar\\source\\repos\\Bridge-Online\\web\\templates";

void ServerMain()
{
	crow::SimpleApp app;

	crow::mustache::set_global_base(TemplatesDir);

	CROW_ROUTE(app, "/")([]()
	{
			return crow::mustache::load_text("index.html");
	});


	app.port(18080).multithreaded().run();
}