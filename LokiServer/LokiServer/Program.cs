var builder = WebApplication.CreateBuilder(args);

// Add services to the container.

builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

app.UseCors(builder =>
{
    builder
       .WithOrigins("http://192.168.1.116:3000")
       .SetIsOriginAllowedToAllowWildcardSubdomains()
       .AllowAnyMethod()
       .AllowAnyHeader()
       .AllowAnyOrigin()
       .WithMethods("GET", "PUT", "POST", "DELETE", "OPTIONS")
       .SetPreflightMaxAge(TimeSpan.FromSeconds(3600));


}
);

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseAuthorization();

app.MapControllers();

app.Run();
