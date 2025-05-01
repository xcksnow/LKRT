import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ListProcessComponent } from './list-process.component';

describe('ListProcessComponent', () => {
  let component: ListProcessComponent;
  let fixture: ComponentFixture<ListProcessComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ListProcessComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ListProcessComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
