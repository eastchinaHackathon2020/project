import request from '../util/request';

export function merge(form) {
  return request({
    url: '/upload/merge',
    method: 'post',
    data: form
  });
}

export function getRecordListAPI() {
  return request({
    url: '/upload/4/getUserRecord',
    method: 'get'
  });
}
