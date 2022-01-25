FROM python:3.9-slim-bullseye
LABEL Maintainer="pierre.fromager"
# System context
USER root
RUN apt-get update
RUN apt-get install curl -y
RUN apt-get install fontconfig -y
RUN apt-get install vim -y
RUN curl -sL https://deb.nodesource.com/setup_14.x | bash
RUN apt-get -y install nodejs
RUN apt-get -y install sudo
# User context
RUN adduser pca
RUN usermod -aG sudo pca
USER pca
ENV MY_HOME /home/pca
WORKDIR ${MY_HOME}
ENV PATH="${MY_HOME}/.local/bin:${PATH}" 
COPY --chown=pca:pca ./package.json ${MY_HOME}/package.json
RUN npm install
COPY --chown=pca:pca ./pca_species.py ${MY_HOME}/pca_species.py
RUN python -m pip install --upgrade pip --user
COPY --chown=pca:pca ./requirements.txt ${MY_HOME}/requirements.txt
RUN python -m pip install -r requirements.txt
RUN npm install vega-lite vega-cli canvas
CMD [ "python", "pca_species.py"]
