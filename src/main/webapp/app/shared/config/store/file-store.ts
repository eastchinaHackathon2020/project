import { Module } from 'vuex';

export const fileStore: Module<any, any> = {
  state: {
    content: '',
    warning: ''
  },
  getters: {
    content: state => state.content,
    warning: state => state.warning
  },
  mutations: {
    setPassage(state, content) {
      state.content = content;
    },
    setWarning(state, warning) {
      state.warning = warning;
    }
  }
};
