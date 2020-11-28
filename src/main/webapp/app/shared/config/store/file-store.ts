import { Module } from 'vuex';

export const fileStore: Module<any, any> = {
  state: {
    thisProject: {}
  },
  getters: {
    thisProject: state => state.thisProject,
  },
  mutations: {
    setProject(state, thisProject) {
      state.thisProject = thisProject;
    },

  }
};
