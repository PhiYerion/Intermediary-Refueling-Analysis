FROM fedora:latest

RUN dnf install -y git g++ cmake mpfr-devel

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

CMD ["/Intermediary-Refueling-Analysis/build/ira", "/Intermediary-Refueling-Analysis/build/tests"]
