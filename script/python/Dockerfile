FROM python:3.9-slim-bullseye
LABEL Maintainer="pierre.fromager"
USER root
WORKDIR /home/app
COPY ./package.json /home/app/package.json
RUN apt-get update
RUN apt-get install curl -y
RUN apt-get install fontconfig -y
RUN apt-get install vim -y
RUN curl -sL https://deb.nodesource.com/setup_14.x | bash
RUN apt-get -y install nodejs
RUN npm install
COPY ./pca_species.py /home/app/pca_species.py
RUN python -m pip install --upgrade pip
COPY ./requirements.txt /home/app/requirements.txt
RUN pip install -r requirements.txt
RUN npm install vega-lite vega-cli canvas
CMD [ "python", "/home/app/pca_species.py"]
