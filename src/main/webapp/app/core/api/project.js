import request from '../util/request';

export function registerProject(form) {
  return request({
    url: '/project/registerProject',
    method: 'post',
    data: form
  });
}


export function getUserProject(userId) {
    return request({
      url: `/userProject/${userId}`,
      method: 'get',
    });
  }
      