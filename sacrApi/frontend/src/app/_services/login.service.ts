import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { environment } from '../../environments/environment';
import { Login, EstaAutenticado_i } from '../_interfaces/login';

@Injectable({
  providedIn: 'root'
})
export class LoginService {

  private url: string = environment.apiUrl;

  constructor(private http: HttpClient) { }

  login(user: string, password: string): Observable<Login> {
    let obj: any = {
      usuario: user,
      password: password
    }

    return this.http.post<Login>(this.url + '/login/', obj);
  }

  estaAutenticado(): Observable<EstaAutenticado_i> {
    return this.http.get<EstaAutenticado_i>(this.url + '/esta-autenticado/');
  }

  logout(): Observable<any> {
    return this.http.get<any>(this.url + '/logout/');
  }
}
