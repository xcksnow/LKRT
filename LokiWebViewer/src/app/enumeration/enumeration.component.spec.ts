import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EnumerationComponent } from './enumeration.component';

describe('EnumerationComponent', () => {
  let component: EnumerationComponent;
  let fixture: ComponentFixture<EnumerationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [EnumerationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(EnumerationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
