#LABEL maintainer="skycoder42.de@gmx.de"
FROM debian:latest

COPY . /tmp/src
RUN /tmp/src/server/dockerbuild/install.sh

# create env vars & ready for usage
ENV KPTSERVER_CONFIG_PATH=/etc/Skycoder42/kptransfer-server.conf
ENV KPTSERVER_HOST=localhost
ENV KPTSERVER_PORT=27352
ENV KPTSERVER_LOCAL=false
ENV KPTSERVER_TIMEOUT=30000

CMD ["/usr/bin/env_start.sh"]
