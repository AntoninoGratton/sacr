import { Component, OnInit } from '@angular/core';
import { NgxSpinnerService } from "ngx-spinner";

import { LoginService } from '../../_services/login.service';
import { FormGroup, FormControl } from '@angular/forms';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.scss']
})
export class LoginComponent implements OnInit {

  public loginForm: FormGroup = new FormGroup({
    user: new FormControl(''),
    pass: new FormControl('')
  });

  constructor(private spinner: NgxSpinnerService,
              private loginApi: LoginService) { }

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
        console.log(resp);
        this.spinner.hide();
      },
      error: (error) => {
        console.log(error);
        this.spinner.hide();
      }
    });
  }

}
