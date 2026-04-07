import pygame

pygame.init()
win = pygame.display.set_mode((400, 400))
pygame.display.set_caption("Zad 2 Wariant 0 (Koło i Kwadrat)")

BIAŁY = (255, 255, 255)
CZARNY = (0, 0, 0)
ŻÓŁTY = (255, 255, 0)

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    win.fill(BIAŁY)

    pygame.draw.circle(win, CZARNY, (200, 200), 120)
    
    pygame.draw.rect(win, ŻÓŁTY, (140, 140, 120, 120))

    pygame.display.update()

pygame.quit()