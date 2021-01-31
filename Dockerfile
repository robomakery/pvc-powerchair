FROM ros:noetic

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -q -y \
        curl  \
        git   \
        gnupg \
        groff \
        jq    \
        less  \
        tree  \
        unzip \
        vim   \
        nano  \
        gedit \
        wget  \
        && rm -rf /var/lib/apt/lists/*

RUN wget http://packages.osrfoundation.org/gazebo.key
RUN apt-key add gazebo.key
RUN apt-get update && apt-get install -y      \
        ros-noetic-desktop-full              \
        ros-noetic-joint-state-publisher-gui \
        ros-noetic-gazebo-plugins            \
        python3-pip                           \
        python3-apt                           \
        python3-vcstool                       \
        python3-colcon-common-extensions      \
        && rm -rf /var/lib/apt/lists/*
RUN rosdep update

# colcon
RUN pip3 install -U setuptools
RUN pip3 install colcon-ros-bundle

WORKDIR /code

# https://docs.docker.com/engine/faq/#why-is-debian_frontendnoninteractive-discouraged-in-dockerfiles
ENV DEBIAN_FRONTEND=newt

COPY docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]

# default command
CMD ["bash"]
