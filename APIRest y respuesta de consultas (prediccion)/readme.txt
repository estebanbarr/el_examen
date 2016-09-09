Explicación de archivos:
- constantes.php: Contiene la información para conectarse a la DB.

- info.php: Primera parte para responder a las predicciones solicitadas en el examen, basicamente se selecciona en que tipo de años responder, es decir, los 10 años de qué planeta, porque los tres planetas tienen distintas cantidades de días al año solar y el enunciado no especifica en que tipo de año basarse.
- respuestas.php: Segunda parte para responder a las predicciones solicitadas en el examen, basicamente son las respuestas una vez seleccionado el tipo de año.

- index.php: Contiene la API Rest.

- Slim: Framework para resolver fácilmente el requerimiento REST.