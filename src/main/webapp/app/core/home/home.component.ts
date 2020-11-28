import Component from 'vue-class-component';
import { Inject, Vue } from 'vue-property-decorator';
import LoginService from '@/account/login.service';
import LoginForm from '../../account/login-form/login-form.vue';

@Component({
  components: {
    'login-form': LoginForm
  }
})
export default class Home extends Vue {
  private loginFormVisible: boolean = false;

  @Inject('loginService')
  private loginService: () => LoginService;

  public openLogin(): void {
    this.loginService().openLogin((<any>this).$root);
  }

  public jump() {
    if (this.authenticated) {
      this.$router.push('/upload');
    } else {
      this.openLogin();
    }
  }

  public get authenticated(): boolean {
    return this.$store.getters.authenticated;
  }

  public get username(): string {
    return this.$store.getters.account ? this.$store.getters.account.login : '';
  }
}
