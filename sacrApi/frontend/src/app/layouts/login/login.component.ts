import { Component, OnInit } from '@angular/core';
import { NgxSpinnerService } from "ngx-spinner";
import { CookieService } from 'ngx-cookie-service';
import { Router } from '@angular/router';

import { LoginService } from '../../_services/login.service';
import { Login } from '../../_interfaces/login';
import { FormGroup, FormControl } from '@angular/forms';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.scss']
})
export class LoginComponent implements OnInit {

  public login: Login;
  public loginForm: FormGroup = new FormGroup({
    user: new FormControl(''),
    pass: new FormControl('')
  });

  constructor(private spinner: NgxSpinnerService,
              private loginApi: LoginService,
              private cookies: CookieService,
              private router: Router) { }

  ngOnInit(): void {
  }
  
  get usuario() {
    return this.loginForm.get('user').value;
  }

  get pass() {
    return this.loginForm.get('pass').value;
  }

  ingresar(): void {
    this.spinner.show();

    this.loginApi.login(this.usuario, this.pass).subscribe({
      next: (resp) => {
        this.login = resp;
        this.cookies.set('user', this.usuario, undefined, '/', undefined, false, 'Lax');
        this.router.navigate(['dashboard']);
        this.spinner.hide();
      },
      error: (error) => {
        console.log(error);
        this.spinner.hide();
      }
    });
  }

}
