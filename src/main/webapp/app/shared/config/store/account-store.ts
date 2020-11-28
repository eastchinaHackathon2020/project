import { Module } from 'vuex';

export const accountStore: Module<any, any> = {
  state: {
    logon: false,
    userIdentity: null,
    authenticated: false,
    ribbonOnProfiles: '',
    activeProfiles: '',
    loginForm: false
  },
  getters: {
    logon: state => state.logon,
    account: state => state.userIdentity,
    authenticated: state => state.authenticated,
    activeProfiles: state => state.activeProfiles,
    ribbonOnProfiles: state => state.ribbonOnProfiles,
    loginForm: state => state.loginForm
  },
  mutations: {
    authenticate(state) {
      state.logon = true;
    },
    authenticated(state, identity) {
      state.userIdentity = identity;
      state.authenticated = true;
      state.logon = false;
    },
    logout(state) {
      state.userIdentity = null;
      state.authenticated = false;
      state.logon = false;
    },
    setActiveProfiles(state, profile) {
      state.activeProfiles = profile;
    },
    setRibbonOnProfiles(state, ribbon) {
      state.ribbonOnProfiles = ribbon;
    },
    setLoginForm(state, loginForm) {
      state.loginForm = loginForm;
    }
  }
};
