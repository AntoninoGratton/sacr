import { Component, OnInit } from "@angular/core";
import { Router } from '@angular/router';

import { LoginService } from '../../_services/login.service';
import { CookieService } from 'ngx-cookie-service';
import { EstaAutenticado_i } from '../../_interfaces/login';

@Component({
  selector: "app-admin-layout",
  templateUrl: "./admin-layout.component.html",
  styleUrls: ["./admin-layout.component.scss"]
})
export class AdminLayoutComponent implements OnInit {
  public sidebarColor: string = "blue";
  private estaAutenticado: EstaAutenticado_i;

  constructor(private loginApi: LoginService,
              private cookies: CookieService,
              private router: Router) {}
  changeSidebarColor(color){
    var sidebar = document.getElementsByClassName('sidebar')[0];
    var mainPanel = document.getElementsByClassName('main-panel')[0];

    this.sidebarColor = color;

    if(sidebar != undefined){
        sidebar.setAttribute('data',color);
    }
    if(mainPanel != undefined){
        mainPanel.setAttribute('data',color);
    }
  }
  changeDashboardColor(color){
    var body = document.getElementsByTagName('body')[0];
    if (body && color === 'white-content') {
        body.classList.add(color);
    }
    else if(body.classList.contains('white-content')) {
      body.classList.remove('white-content');
    }
  }
  ngOnInit() {
    var body = document.getElementsByTagName('body')[0];
    body.classList.add('black-content');
    this.changeSidebarColor('blue');
    this.loginApi.estaAutenticado().subscribe({
      next: (resp) => {
        this.estaAutenticado = resp;

        if(this.cookies.get("user") == '')
        {
            this.router.navigate(['login']);
        }

        else if(this.estaAutenticado.autenticado == false) {
            console.log("Entra aaaaaa");
            this.cookies.delete('user', '/');
            this.router.navigate(['login']);
        }
      },
      error: (error) => {
        console.log("Falla: " + error);
      }
    });
  }
}
