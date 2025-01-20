# Use an official Ubuntu as the base image
FROM ubuntu:24.04

# Set the maintainer label (optional)
LABEL maintainer="gabe.susman@gmail.com"

# Update the package list and install essential tools
RUN apt-get update && apt-get install -y \
    build-essential \   # Includes g++, gcc, make, etc.
    cmake \             # For projects using CMake
    git \               # Optional: For cloning repositories
    && apt-get clean    # Clean up to reduce image size

# Set the working directory inside the container
WORKDIR /app

# Copy your C++ source code into the container
# Replace '.' with the path to your local source directory
COPY . /app

# Optional: Set the default command to run
CMD ["bash"]
