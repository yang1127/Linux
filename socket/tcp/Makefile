all:tcp_thread tcp_cli
tcp_thread:tcp_thread.cpp
	g++ $^ -o $@ -lpthread
tcp_process:tcp_process.cpp
	g++ $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
