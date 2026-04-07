import pygame
import math


pygame.init()
width, height = 600, 600
win = pygame.display.set_mode((width, height))
pygame.display.set_caption("Zadanie 1 - Wariant 12 (12-kąt i transformacje)")

ZIELONY = (0, 255, 0)
ŻÓŁTY_BG = (255, 255, 0) 

r = 150
polygon_surface = pygame.Surface((300, 300), pygame.SRCALPHA)
center = (150, 150)
points = []

for i in range(12):
    angle = 2 * math.pi * i / 12
    x = center[0] + int(r * math.cos(angle))
    y = center[1] + int(r * math.sin(angle))
    points.append((x, y))

pygame.draw.polygon(polygon_surface, ZIELONY, points)

current_surface = polygon_surface
current_rect = current_surface.get_rect(center=(width // 2, height // 2))

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        
        elif event.type == pygame.KEYDOWN:
            
            if event.key == pygame.K_0:
                current_surface = polygon_surface
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_1:
                current_surface = pygame.transform.scale(polygon_surface, (int(300 * 0.35), int(300 * 0.35)))
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_2:
                current_surface = pygame.transform.rotate(polygon_surface, 45)
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_3:
                current_surface = pygame.transform.flip(polygon_surface, False, True)
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_4:
                scale = pygame.transform.scale(polygon_surface, (int(300 * 0.35), 300))
                current_surface = pygame.transform.rotozoom(scale, 45, 1)
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_5:
                current_surface = pygame.transform.scale(polygon_surface, (300, int(300 * 0.35)))
                current_rect = current_surface.get_rect(midtop=(width // 2, 0)) 
                
            elif event.key == pygame.K_6:
                scale = pygame.transform.scale(polygon_surface, (int(300 * 0.35), 300))
                current_surface = pygame.transform.rotozoom(scale, 180, 1)
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_7:
                scale = pygame.transform.scale(polygon_surface, (int(300 * 0.5), 300))
                current_surface = pygame.transform.flip(scale, True, False)
                current_rect = current_surface.get_rect(center=(width // 2, height // 2))
                
            elif event.key == pygame.K_8:
                scale = pygame.transform.scale(polygon_surface, (300, int(300 * 0.4)))
                current_surface = pygame.transform.rotate(scale, -20)
                current_rect = current_surface.get_rect(center=(width // 2 - 100, height - 100)) 
                
            elif event.key == pygame.K_9:
                scale = pygame.transform.scale(polygon_surface, (int(300 * 0.35), 300))
                current_surface = pygame.transform.rotozoom(scale, 90, 1)
                current_rect = current_surface.get_rect(center=(width // 2 + 100, height // 2))

    win.fill(ŻÓŁTY_BG) 
    win.blit(current_surface, current_rect)
    pygame.display.flip()

pygame.quit()