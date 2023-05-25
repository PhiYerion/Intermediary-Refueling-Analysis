FROM debian:latest

RUN apt update; apt install -y git cmake g++ libmpfr-dev

RUN git clone https://github.com/PhiYerion/Intermediary-Refueling-Analysis.git && \
    cd Intermediary-Refueling-Analysis/ && \
    git submodule init && \
    git submodule update && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . && \
    mv ira /bin && \
    mv tests /bin

CMD ["./ira", "./tests"]
