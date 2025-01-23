FROM ubuntu:24.04
LABEL maintainer="gabe.susman@gmail.com"
RUN apt-get update && apt-get install -y \
    build-essential \   # Includes g++, gcc, make, etc.
    cmake \             # For projects using CMake
    git \               # Optional: For cloning repositories
    && apt-get clean    # Clean up to reduce image size
WORKDIR /app
COPY . /app
CMD ["bash"]
