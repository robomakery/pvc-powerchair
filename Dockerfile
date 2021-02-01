FROM ros:noetic

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -q -y \
        curl  \
        gedit \
        git   \
        gnupg \
        groff \
        jq    \
        less  \
        nano  \
        tree  \
        unzip \
        vim   \
        wget  \
        && rm -rf /var/lib/apt/lists/*

RUN wget http://packages.osrfoundation.org/gazebo.key
RUN apt-key add gazebo.key
RUN apt-get update && apt-get install -y     \
        python3-apt                          \
        python3-colcon-common-extensions     \
        python3-pip                          \
        python3-vcstool                      \
        ros-noetic-desktop-full              \
        ros-noetic-gazebo-plugins            \
        ros-noetic-joint-state-publisher-gui \
        ros-noetic-key-teleop                \
        && rm -rf /var/lib/apt/lists/*
RUN rosdep update

WORKDIR /code

# https://docs.docker.com/engine/faq/#why-is-debian_frontendnoninteractive-discouraged-in-dockerfiles
ENV DEBIAN_FRONTEND=newt

COPY docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]

# default command
CMD ["bash"]
