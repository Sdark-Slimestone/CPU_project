# 生成 Verilog 硬件文件
genv:
	sbt run
	mkdir -p verilog
	mv -f *.sv verilog/
	cp src/main/resources/DPI_Memory.v verilog/
	rm -f DPI_Memory.v
	verilator --lint-only -Wall -Wno-UNUSEDSIGNAL -Wno-MULTITOP verilog/*.sv verilog/DPI_Memory.v

# 运行测试
test:
	sbt test

# 清理编译文件
clean:
	sbt clean
	rm -rf verilog

rebuild: clean genv