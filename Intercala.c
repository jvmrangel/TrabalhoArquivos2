#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char** argv)
{
	FILE *a, *b, *saida;
	Endereco ea, eb;
   char buffer[30];
   char novoNome[]= "cep_ordenado_final.dat";
   int inicio= 0, final=16;

	
   while( inicio<final-1)
   {
      sprintf(buffer, "cep_%d.dat", inicio);
      a = fopen(buffer,"r");

      sprintf(buffer, "cep_%d.dat", inicio+1);
      b = fopen(buffer,"r");

      sprintf(buffer, "cep_%d.dat", final);
      saida = fopen(buffer,"w");

      fread(&ea,sizeof(Endereco),1,a);
      fread(&eb,sizeof(Endereco),1,b);

      while(!feof(a) && !feof(b))
      {
         if(compara(&ea,&eb)<0) // ea.cep < eb.cep
         {
            fwrite(&ea,sizeof(Endereco),1,saida);
            fread(&ea,sizeof(Endereco),1,a);
         }
         else
         {
            fwrite(&eb,sizeof(Endereco),1,saida);
            fread(&eb,sizeof(Endereco),1,b);
         }
      }

      while(!feof(a))
      {
         fwrite(&ea,sizeof(Endereco),1,saida);
         fread(&ea,sizeof(Endereco),1,a);		
      }

      while(!feof(b))
      {
         fwrite(&eb,sizeof(Endereco),1,saida);
         fread(&eb,sizeof(Endereco),1,b);		
      }

      printf("bloco %d  + bloco %d  = bloco %d intercalado com sucesso\n", inicio, inicio+1, final);

      fclose(a);
      fclose(b);
      fclose(saida);
      final=final+1;
      inicio=inicio+2;
                  
   }

   sprintf(buffer, "cep_%d.dat", final-1);

   if(rename(buffer, novoNome) == 0) 
   {
      printf("Arquivo renomeado com sucesso");
   } 
   else 
   {
      printf("Erro: falha ao renomear o arquivo");
   }
      

   return 0;
	
}



