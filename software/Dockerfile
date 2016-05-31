FROM ros:jade-ros-base
MAINTAINER Dylan Vaughn dylan@robomakery.com

ENV PVC_VERSION 0.1.0

RUN apt-get update && apt-get install -y \
    ros-jade-robot=1.2.0-0* \
    ros-jade-perception=1.2.0-0* \
    python-pip \
    vim \
    emacs \
    wget \
    git \
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

# Install python dependencies
RUN pip install -r /usr/src/app/requirements.txt

# Expose ROS port
EXPOSE 11311

# remove old entrypoint
ENTRYPOINT []

# Build ROS packages
RUN . /opt/ros/jade/setup.sh \
&& cd /usr/src/app/ros \
&& catkin_make

# run ros
CMD ["/usr/src/app/run-ros.sh"]
