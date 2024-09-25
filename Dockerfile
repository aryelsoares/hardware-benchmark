# GCC Compiler
FROM gcc:latest

# Install dependencies
RUN apt-get update && \
    apt-get install -y libsensors4-dev && \
    apt-get install -y curl && \
    curl -o /usr/local/bin/wait-for-it.sh https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh && \
    chmod +x /usr/local/bin/wait-for-it.sh

# Working Folder
WORKDIR /usr/src/hardware-info/

# Get main files
COPY cpu_info.hpp gpu_info.hpp main.cpp mem_info.hpp temp_info.hpp ./

# Get influxdb.hpp library
RUN curl -o /usr/src/hardware-info/influxdb.hpp https://raw.githubusercontent.com/TheYonkk/influxdb-cpp-2/master/influxdb.hpp

# Compile
RUN g++ -o benchmark main.cpp -lsensors

# Execute
CMD ["/usr/local/bin/wait-for-it.sh", "influxdb:8086", "--", "./benchmark"]
