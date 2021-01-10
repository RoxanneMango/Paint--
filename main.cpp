#include "paint.h"

int
main(int argc, char ** argv)
{
	try
	{
		std::unique_ptr<Paint> paint = std::make_unique<Paint>("orders.txt", "orders.txt", 25, "icons.png");
		
		paint->load();
		while(paint->isRunning)
		{
			paint->handleEvents();
			paint->update();
			paint->render();
		}
		paint->save();

		paint.reset(nullptr);
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
		return -1;
	}
	catch( ... )
	{
		printf("Something went wrong\n");
		return -1;
	}

	return 0;
}