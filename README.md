## Docker

### Сборка

```bash
docker build -t satellite_sim .
```

### Запуск (интерактивный ввод)

```bash
docker run --rm -it satellite_sim
```

### Примечания

- Используется `Dockerfile` в корне проекта.
- Если нужно собрать с `DockerFile`, используйте:

```bash
docker build -f DockerFile -t satellite_sim .
```
