FROM ubuntu:20.04 AS main_development_image

ARG USER=developer
ARG GROUP=developer

ENV DEBIAN_FRONTEND noninteractive

# Setup environment
ENV TERM=linux
ENV DEBIAN_FRONTEND=noninteractive
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

# Copy installation scripts, and perform the dependency installation
COPY docker/install.sh .
COPY docker/requirements.txt .
COPY docker/requirements_pip.txt .

RUN ./install.sh
RUN rm install.sh requirements.txt requirements_pip.txt

ARG USERID=1003

RUN addgroup --gid 1000 $GROUP \
 && adduser --uid 1000 --ingroup $GROUP --home /home/$USER --shell /bin/sh --disabled-password --gecos "" $USER \
 && adduser $USER sudo \
 && adduser $USER dialout \
 && echo "$USER ALL=NOPASSWD: ALL" >> /etc/sudoers.d/$USER

USER $USER:$GROUP

ENV USER_WS /home/$USER/ws
RUN mkdir -p $USER_WS

WORKDIR $USER_WS

COPY --chown=$USER:$GROUP docker/files/DOTaliases /home/$USER/.bash_aliases

RUN mkdir -p $USER_WS/src

COPY --chown=root:root docker/files/fixuid_config.yml /etc/fixuid/config.yml
RUN /bin/bash -c '\
     ARCH=`uname -m` && if [ "$ARCH" == "aarch64" ]; then FIXUID_ARCH="arm64"; else FIXUID_ARCH="amd64"; fi \
  && curl -SsL https://github.com/boxboat/fixuid/releases/download/v0.6.0/fixuid-0.6.0-linux-$FIXUID_ARCH.tar.gz | sudo tar -C /usr/local/bin -xzf - \
  && sudo chmod 4755 /usr/local/bin/fixuid \
  && cd /etc/fixuid \
  && sudo sed -i "s/_USER_/$USER/" config.yml \
  && sudo sed -i "s/_GROUP_/$GROUP/" config.yml \
  && echo Done installing fixuid for $FIXUID_ARCH'

#
# Final setup of the workspace

COPY docker/files/developrc /home/developer/.developrc
RUN echo "source /home/developer/.developrc" >> /home/developer/.bashrc

ENV WITHIN_DEV 1

ENV SHELL /bin/bash
ENTRYPOINT ["fixuid", "-q", "/bin/bash"]