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

# Create headers directory
RUN mkdir -p headers

# Get main files
COPY main.cpp ./
COPY headers/ ./headers/

# Get influxdb.hpp library
RUN curl -o ./headers/influxdb.hpp https://raw.githubusercontent.com/TheYonkk/influxdb-cpp-2/master/influxdb.hpp

# Compile
RUN g++ -o benchmark main.cpp -I./headers -lsensors

# Execute
CMD ["/usr/local/bin/wait-for-it.sh", "influxdb:8086", "--", "./benchmark"]
