import request from '../util/request';

export function get() {
  return request({
    url: '/users',
    method: 'get'
  });
}

export function getOne(id) {
  return request({
    url: '/users/' + id,
    method: 'get'
  });
}

export function post(userName, password) {
  return request({
    url: '/users',
    method: 'post',
    data: {
      userName,
      password
    }
  });
}

export function put(id, userName, password) {
  return request({
    url: '/users',
    method: 'put',
    data: {
      id,
      userName,
      password
    }
  });
}

export function del(id) {
  return request({
    url: '/users',
    method: 'delete',
    params: { id }
  });
}
