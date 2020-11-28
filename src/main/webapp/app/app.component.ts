import Vue from 'vue';
import Component from 'vue-class-component';
import Ribbon from './core/ribbon/ribbon.vue';
import LoginForm from './account/login-form/login-form.vue';
import jhiNavbar from './core/jhi-navbar/jhi-navbar.vue';

@Component({
  components: {
    ribbon: Ribbon,
    'login-form': LoginForm,
    'jhi-navbar': jhiNavbar
  }
})
export default class App extends Vue {}
