/* Welcome to Shoot The Rect*/

#include <Gamebuino-Meta.h>

int posX = 10, posY = 31, enPosY = 1, enPosX = 80;
int speedX = 1, ennSpeedX = 1, trajShoot = 1;
int shootTblX[200], shootTblY[200], enemies_posY[50], enemies_posX[50];
int enSizeL = 8, enSizeW = 3, shootSizeL = 3, shootSizeW = 1;
int timer3 = 0, timer = 0, y = 0, timer2 = 0;
int health = 3, score = 0, storedScore = 0;
bool kill = false;
bool ultimateReady = false, ultimateActivated = false;

void initialization()
{
  ennSpeedX = 1;
  score = 0;
  posX = 10;
  posY = 31;
  health = 3;
  shootSizeL = 3;
  shootSizeW = 1;
  storedScore = 0;
  ultimateReady = false;
  ultimateActivated = false;
  enPosY = random(1, 63);

  for (int i = 0; i < 200; i++)
  {
    shootTblX[i] = -200;
    shootTblY[i] = -100;
  }
  for (int i = 0; i < 50; i++)
  {
    enemies_posY[i] = -500;
    enemies_posX[i] = -600;
  }
}

void setup()
{
  gb.begin();
  initialization();
}

void displayPlayer(int x, int y)
{
  /*White when full life, yellow when 2, ticking red when 1 left*/
  if (health == 3)
  {
    gb.display.setColor(WHITE);
  }
  else if (health == 2)
  {
    gb.display.setColor(YELLOW);
  }
  else if (health == 1)
  {
    if (timer == 5)
    {
      gb.display.setColor(WHITE);
      timer = 0;
    }
    else
    {
      gb.display.setColor(RED);
      timer++;
    }
  }
  /*Ship sprite*/
  gb.display.fillRect(x, y, 3, 3);
  gb.display.fillRect(x, y + 1, 7, 1);
  gb.display.fillRect(x - 3, y - 1, 3, 5);
  gb.display.fillRect(x - 4, y - 3, 1, 9);
  gb.display.fillRect(x - 5, y - 2, 1, 2);
  gb.display.fillRect(x - 5, y + 3, 1, 2);
  gb.display.setColor(BLACK);
  gb.display.fillRect(x - 2, y + 1, 1, 1);
  gb.display.setColor(WHITE);
}

void initShoot()
/*Activate one bullet and set its position to the ship cannon*/
{
  for (int i = 0; i < 200; i++)
  {
    if (shootTblX[i] < 0)
    {
      shootTblX[i] = posX + 4;
      shootTblY[i] = posY + 1;
      break;
    }
  }
}

void moveShoot()
/*Make every activated bullet move to the left*/
{
  for (int i = 0; i < 200; i++)
  {
    if (shootTblX[i] > 0)
    {
      gb.display.setColor(YELLOW);
      gb.display.fillRect(shootTblX[i], shootTblY[i], shootSizeL, shootSizeW);
      shootTblX[i] = shootTblX[i] + 2;
      /*De-activate bullet if it pass the left border*/
      if (shootTblX[i] > 79)
      {
        shootTblX[i] = -200;
        shootTblY[i] = -100;
      }
    }
  }
}

void ennemy()
{
  for (int i = 0; i < 50; i++)
  {
    /*Spawn ennemies at random places between a given x and y*/
    if (enemies_posY[i] < 0)
    {
      enemies_posY[i] = random(5, 63);
      enemies_posX[i] = random(79, 1000);
      break;
    }
    else
    {
      continue;
    }
  }

  for (int i = 0; i < 50; i++)
  {
    if (enemies_posY[i] > 0)
    {
      /*Accelerate with the increasing score*/
      if (score >= 25 && score < 50)
      {
        gb.display.setColor(GREEN);
      }
      if (score >= 50)
      {
        gb.display.setColor(PURPLE);
      }

      /*Display the correspounding ennemy and make it move*/
      gb.display.fillRect(enemies_posX[i], enemies_posY[i], enSizeL, enSizeW);
      enemies_posX[i] = enemies_posX[i] - ennSpeedX;
      gb.display.setColor(WHITE);

      /*Decrease life if an ennemy touch the player hitbox*/
      if (gb.collide.rectRect(enemies_posX[i], enemies_posY[i], enSizeL, enSizeW, posX, posY + 1, 7, 1) ||
          gb.collide.rectRect(enemies_posX[i], enemies_posY[i], enSizeL, enSizeW, posX - 5, posY - 3, 2, 9))
      {
        health--;
        enemies_posY[i] = -500;
      }

      /*De-Activate the correspouding ennemy if it pass the left border*/
      if (enemies_posX[i] <= 0 - enSizeL)
      {
        enemies_posY[i] = -500;
        enemies_posX[i] = -600;
      }

      for (int j = 0; j < 50; j++)
      {
        /*De-Activate the correspouding ennemy and the correspouding bullet if a player shoot on him*/
        if (gb.collide.rectRect(enemies_posX[i], enemies_posY[i], enSizeL, enSizeW, shootTblX[j], shootTblY[j], shootSizeL, shootSizeW))
        {
          enemies_posY[i] = -500;
          enemies_posX[i] = -600;
          shootTblX[j] = -200;
          shootTblY[j] = -100;
          kill = true;
        }
      }
    }
  }
}

void playerDeath()
/*Death message with final score if health drop to 0*/
{
  if (health == 0)
  {
    gb.display.clear();
    gb.display.println();
    gb.display.print("Score : ");
    gb.display.setColor(BLUE);
    gb.display.println(score);
    gb.display.setColor(WHITE);
    gb.display.println();
    gb.display.println("Vous etes mort,");
    gb.display.println("veuillez appuyer surla touche menu de");
    gb.display.println("gauche pour");
    gb.display.println("recommencer");
    if (gb.buttons.pressed(BUTTON_MENU))
    {
      initialization();
    }
  }
}

void playerMoves()
{
  if (gb.buttons.repeat(BUTTON_DOWN, 0))
  {
    posY = posY + 2;
  }
  if (gb.buttons.repeat(BUTTON_UP, 0))
  {
    posY = posY - 2;
  }
  if (gb.buttons.repeat(BUTTON_LEFT, 0))
  {
    posX = posX - 2;
  }
  if (gb.buttons.repeat(BUTTON_RIGHT, 0))
  {
    posX = posX + 2;
  }
}

void borderTp()
/*If you pass the top border, ship is teleported to the bottom border and vice versa.
You cannot pass the left and the right border*/
{
  if (posY < 0)
  {
    posY = 62;
  }

  if (posY > 63)
  {
    posY = 0;
  }

  if (posX < 0)
  {
    posX = 0;
  }

  if (posX > 79)
  {
    posX = 79;
  }
}

void shoots()
/*Handle both spam press button and keeping*/
{
  if (gb.buttons.repeat(BUTTON_A, 0))
  {
    timer3++;
    if (timer3 == 5)
    {
      initShoot();
      timer3 = 0;
    }
  }

  if (gb.buttons.pressed(BUTTON_A))
  {
    initShoot();
  }
}

void ultimate()
/*Every 25 enemy killed, the player can activate an ultimate
that increases the size of the bullet for 250 iteration*/
{
  if (score == storedScore + 40)
  {
    storedScore = score;
    ultimateReady = true;
  }
  if (gb.buttons.pressed(BUTTON_B) && ultimateReady)
  {
    ultimateActivated = true;
  }
  if (ultimateActivated)
  {
    y++;
    if (y < 250)
    {
      shootSizeL = 6;
      shootSizeW = 5;
    }
    else
    {
      shootSizeL = 3;
      shootSizeW = 1;
      y = 0;
      ultimateActivated = false;
      ultimateReady = false;
      gb.display.setColor(WHITE);
    }
  }

  /*When the utilmate is ready, we show a big "U" next to the health points*/
  if (ultimateReady)
  {
    gb.display.setCursor(33, 0);
    gb.display.setColor(YELLOW);
    if (timer2 == 5 && ultimateActivated)
    {
      gb.display.setColor(WHITE);
      timer2 = 0;
    }
    gb.display.print("U");
    gb.display.setColor(WHITE);
    if (ultimateActivated && timer2 != 5)
    {
      gb.display.setColor(ORANGE);
      timer2++;
    }
    gb.display.setCursor(0, 0);
  }
}

void loop()
{
  while (!gb.update())
    ;

  if (score >= 25)
  {
    ennSpeedX = 2;
  }
  if (score >= 50)
  {
    ennSpeedX = 3;
  }

  if (health > 0)
  {
    if (kill)
    {
      score++;
      if (score == 50 || score == 100 || score == 150 || score == 200)
      {
        health++;
      }
      kill = false;
    }

    /*health and score counter*/
    gb.display.clear();
    gb.display.print("health:");
    gb.display.setColor(RED);
    gb.display.print(health);
    gb.display.setCursor(40, 0);
    gb.display.setColor(WHITE);
    gb.display.print("score:");
    gb.display.setColor(BLUE);
    gb.display.print(score);
    gb.display.setCursor(0, 0);
    gb.display.setColor(WHITE);

    ennemy();
    displayPlayer(posX, posY);
    playerMoves();
    borderTp();
    moveShoot();
    shoots();
    ultimate();
  }
  playerDeath();
}
