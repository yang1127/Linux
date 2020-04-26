#include "httplib.h"

void helloworld(const httplib::Request &req, httplib::Response &rsp)
{
	printf("httplib server recv a raq:%s\n", req.path.c_str());
	rsp.set_content("<html><h1>HelloWorld</h1></html>", "text/html");
	rsp.status = 200;
}

int main(int argc, char*argv[] )
{
	//std::vector<Adapter> list;
	//AdapterUtil::GetAllAdapter(&list);

	httplib::Server srv;
	srv.Get("/", helloworld); //srv.Get:函数指针
	srv.listen("0.0.0.0", 9000); //0.0.0.0:本机任意ip地址

	system("pause");
	return 0;
}
