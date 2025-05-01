import { Routes } from '@angular/router';
import { FileExplorerComponent } from './file-explorer/file-explorer.component';
import { ListProcessComponent } from './list-process/list-process.component';
import { EnumerationComponent } from './enumeration/enumeration.component';
import { ConfigurationComponent } from './configuration/configuration.component';

export const routes: Routes = [
    { path: 'explorer/:id', component: FileExplorerComponent },
    { path: 'processes/:id', component: ListProcessComponent },
    { path: 'enumeration/:id', component: EnumerationComponent },
    { path: 'configuration', component: ConfigurationComponent },
];
