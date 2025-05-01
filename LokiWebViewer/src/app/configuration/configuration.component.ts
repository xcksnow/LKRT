import { Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ServerService } from '../server.service';


@Component({
  selector: 'app-configuration',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './configuration.component.html',
  styleUrl: './configuration.component.css'
})

export class ConfigurationComponent {
  serverURLPopup = false;
  url = '';
  message = '';
  isError = false;

  constructor(private serverService: ServerService) {
    this.url = this.serverService.getUrl();
  }

  flashMessage(time: number, message: string, isError: boolean) {
    this.message = message;
    this.isError = isError;
    setTimeout(() => {
      this.message = "";
    }, time);
  }


  editServerURL(){
    console.log("Edit server URL");
    this.serverURLPopup = true;
  }

  closePopup(){
    this.serverURLPopup = false;
  }

  updateURL(){
    if (this.url.endsWith('/')) {
      this.url = this.url.slice(0, -1);
    }
    this.serverURLPopup = false;
    this.serverService.setUrl(this.url);
    this.flashMessage(3000, "Server URL updated successfully", false);
    this.url = this.serverService.getUrl();
  }

}
