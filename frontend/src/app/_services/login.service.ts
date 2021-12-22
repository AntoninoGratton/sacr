import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { environment } from '../../environments/environment';

@Injectable({
  providedIn: 'root'
})
export class LoginService {

  private url: string = environment.apiUrl;

  constructor(private http: HttpClient) { }

  login(user: string, password: string): Observable<any> {
    let obj: any = {
      usuario: user,
      password: password
    }

    return this.http.post<any>(this.url + '/login/', obj);
  }
}
