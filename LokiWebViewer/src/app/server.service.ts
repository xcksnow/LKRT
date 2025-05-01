import { Injectable, Inject, PLATFORM_ID } from '@angular/core';
import { CommonModule, DOCUMENT } from '@angular/common';
import { isPlatformBrowser } from '@angular/common';

@Injectable({
  providedIn: 'root'
})
export class ServerService {
  public url: string = 'http://localhost:1200';
  platformId: any;

  constructor(@Inject(PLATFORM_ID) platformId: Object) {
    this.platformId = platformId;
    if (isPlatformBrowser(this.platformId)) {
      // Use localStorage here
      this.url = localStorage.getItem('url') || this.url;
    }
  }

  getUrl = () => {
    // store in LocalStorage
    if (isPlatformBrowser(this.platformId)) {
      this.url = localStorage.getItem('url') || 'http://localhost';
    }
    return this.url;
  }

  setUrl = (url: string) => {
    if (isPlatformBrowser(this.platformId)) {
      // Use localStorage here
      localStorage.setItem('url', url);
    }
    this.url = url;
  }
}
