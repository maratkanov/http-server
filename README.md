http-server
===========

HTTP Server. Implementing in C++ using Boost Library.

load balancer setup
===========

1. create file in /etc/nginx/sites-available

```
upstream backend  {
  server localhost:8000;
  server localhost:8001;
  server localhost:8002;
}

server {
  location / {
    proxy_pass  http://backend;
  }
}
```

2. create symlink in /etc/nginx/sites-enabled
3. restart nginx

```
service nginx restart
```
