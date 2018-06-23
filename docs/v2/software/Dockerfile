FROM ros:kinetic-ros-base
MAINTAINER Dylan Vaughn dylan@robomakery.com

ENV PVC_VERSION 0.1.0

RUN apt-get update && apt-get install -y \
    apt-utils \
    emacs \
    git \
    python-pip \
    vim \
    wget \
    ros-kinetic-robot=1.3.0-0* \
    ros-kinetic-perception=1.3.0-0* \
&& apt-get clean \
&& rm -rf /var/lib/apt/lists/*

# Create app directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

# Copy app into container
COPY . /usr/src/app

# Remove dynamic files
RUN rm -rf /usr/src/app/ros/devel
RUN rm -rf /usr/src/app/ros/build

# upgrade PIP
RUN pip install --upgrade pip

# Install python dependencies
RUN pip install -r /usr/src/app/requirements.txt

# Install some other ROS packages
RUN apt-get update && apt-get install -y \
    ros-kinetic-teleop-twist-keyboard \
    ros-kinetic-joystick-drivers \
&& apt-get clean \
&& rm -rf /var/lib/apt/lists/*

# These are only needed for simulation
RUN apt-get update && apt-get install -y \
    ros-kinetic-gazebo-ros-control \
&& apt-get clean \
&& rm -rf /var/lib/apt/lists/*

# Expose ROS port
EXPOSE 11311

# remove old entrypoint
ENTRYPOINT []

# run ros
CMD ["/usr/src/app/run-ros.sh"]
