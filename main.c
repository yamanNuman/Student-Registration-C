#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <io.h>

typedef struct student
{
    int studentNo;
    int courseCode;
    int point;
} stud;

void sort(int n)
{
    FILE *fp_index, *fp_data;
    int i, j, record1, record2;
    stud stud1, stud2;

    fp_index = fopen("index.txt", "r+b");
    fp_data = fopen("record.bin", "r+b");

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            fseek(fp_data, sizeof(stud1) * j, SEEK_SET);
            fseek(fp_index, sizeof(int) * j, SEEK_SET);

            fread(&record1, sizeof(int), 1, fp_index);

            fread(&record2, sizeof(int), 1, fp_index);

            if (record1 > record2) //index ve datalarýn swap iþlemi
            {
                fread(&stud1, sizeof(stud1), 1, fp_data);
                fread(&stud2, sizeof(stud2), 1, fp_data);

                fseek(fp_data, (sizeof(stud1)) * j, SEEK_SET);
                fseek(fp_index, (sizeof(int)) * j, SEEK_SET);

                fwrite(&record2, sizeof(int), 1, fp_index);
                fwrite(&record1, sizeof(int), 1, fp_index);

                fwrite(&stud2, sizeof(stud1), 1, fp_data);
                fwrite(&stud1, sizeof(stud1), 1, fp_data);
            }
        }
    }

    fclose(fp_index);
    fclose(fp_data);
}

void add(stud *stud1)
{
    int n = 0;
    float temp;
    FILE *fp_index, *fp_data;

    fp_index = fopen("index.txt", "r+b");
    fp_data = fopen("record.bin", "r+b");

    printf("\nEnter Student No: ");
    scanf("%d", &stud1->studentNo);
    printf("Enter Course Code: ");
    scanf("%d", &stud1->courseCode);
    printf("Enter Point: ");
    scanf("%f", &temp);
    stud1->point = temp;
    printf("\n\n");

    fseek(fp_data, 0, SEEK_END);

    if (fwrite(stud1, sizeof(*stud1), 1, fp_data))
    {
        fseek(fp_index, 0, SEEK_END);
        fwrite(&(stud1->studentNo), sizeof(int), 1, fp_index);
    }
    else
    {
        printf("\n\nCould NOT add !!!");
    }
    rewind(fp_index);
    while ((fread(stud1, sizeof(int), 1, fp_index)))
        n++;

    fclose(fp_index);
    fclose(fp_data);

    if (n > 1)
        sort(n);
}

void edit(int sr, stud *s2, int low, int high)
{
    int r,r2, size, loc, flag = 0, mid, temp;
    char yN;
    //stud s2;
    FILE *fp_index, *fp_record;
    fp_index = fopen("index.txt", "r+b");
    fp_record = fopen("record.bin", "r+b");
    //low = 0;
    //Middle Index
    //printf("%d", r);/

    //fseek(fp_index,sizeof(int), SEEK_END); // seek to end of file
    //size = ftell(fp_index); // get current file pointer
    //printf("Size: %d",size);
    //printf("Eleman Sayisi: %d", size/sizeof(int));
    //high = (size/sizeof(int))-1;
    //printf("High: %d",high);


    //while(low <= high)
    //{


        if (low>high)
        {
            return -1;
        }

        mid = low+(high-low)/2;
        fseek(fp_index, mid*4,SEEK_SET);
        fread(&r, sizeof(int), 1, fp_index);

        if(r == sr)
        {

            fseek(fp_index, (-1)*sizeof(int), SEEK_CUR);
            fread(&r2, sizeof(int), 1, fp_index);

            if(r2<r)
            {
                return mid;
            }

            else
            {
                fseek(fp_index, (-1)*sizeof(int), SEEK_CUR);
                loc = ftell(fp_index);
            //printf("Loc: \n%d",loc);
                fseek(fp_record, loc / sizeof(int) * sizeof(*s2), SEEK_SET);
                fread(s2, sizeof(*s2), 1, fp_record);
                printf("\n\nStudent No :%d", s2->studentNo);
                printf("\n\nCourse Code :%d", s2->courseCode);
                printf("\n\nPoint :%d\n\n", s2->point);
                printf("Do you update this data? (y/n)");
                scanf("%c", &yN);
                if(yN == 'y')
                {

                    printf("\nEnter New Point : ");
                    scanf("%d", &temp);
                    (s2->point) = temp;
                    fseek(fp_record, (-1) * sizeof(*s2), SEEK_CUR);
                    fwrite(s2, sizeof(*s2), 1, fp_record);


                }
                else
                {
                    /*edit(sr, s2, mid+1, high);
                    edit(sr, s2, low, mid-1); */
                    printf("Do you update this data? (y/n)");
                    scanf("%c", &yN);

                    if(yN == 'y')
                    {

                    printf("\nEnter New Point : ");
                    scanf("%d", &temp);
                    (s2->point) = temp;
                    fseek(fp_record, (-1) * sizeof(*s2), SEEK_CUR);
                    fwrite(s2, sizeof(*s2), 1, fp_record);

                    }

                    else
                    {
                        edit(sr, s2, mid+1, high);
                        edit(sr, s2, low, mid-1);
                    }


                    //edit(sr, s2, low, mid-1);

                }


            }


        }

        else if(sr<r)
        {
            //high = mid-1;
            edit(sr, s2, low, mid-1);

        }

        else if(sr>r)
        {
            //low = mid+1;
           edit(sr, s2, mid+1, high);
        }
    //}

    fclose(fp_index);
    fclose(fp_record);
}

void search(stud *s2, int r_n)
{
    int r, loc, flag = 0;

    FILE *fp_index, *fp_record;

    fp_index = fopen("index.txt", "r+b");
    fp_record = fopen("record.bin", "r+b");

    while (fread(&r, sizeof(int), 1, fp_index))
    {
        if (r == r_n)
        {
            flag = 1;

            break;
        }
    }

    if (!flag)
    {
        printf("\n\nRecord NOT found !!!");
    }
    else
    {
        fseek(fp_index, (-1) * sizeof(int), SEEK_CUR);
        loc = ftell(fp_index);
        fseek(fp_record, loc / sizeof(int) * sizeof(*s2), SEEK_SET);
        fread(s2, sizeof(*s2), 1, fp_record);

        printf("\n\nStudent No :%d", s2->studentNo);
        printf("\n\nCourse Code :%d", s2->courseCode);
        printf("\n\nPoint :%d", s2->point);
    }

    printf("\n\n");
    fclose(fp_index);
    fclose(fp_record);
}

void delet(stud *s2, int r_n)
{
    int r, loc, flag = 0,file_handler_ind,file_handler_rec,file_size_ind,file_size_rec;
    char ch;

    FILE *fp_index, *fp_record;

    fp_index = fopen("index.txt", "r+b");
    fp_record = fopen("record.bin", "r+b");

    file_handler_ind=_fileno(fp_index);
    file_handler_rec=_fileno(fp_record);

    file_size_ind=_filelength(file_handler_ind);
    file_size_rec=_filelength(file_handler_rec);

    while (fread(&r, sizeof(int), 1, fp_index))
    {
        if (r == r_n)
        {
            flag = 1;

            break;
        }
    }
    if (!flag)
    {
        printf("\n\nRecord NOT found !!!");
    }
    else
    {
        fseek(fp_index, (-1) * sizeof(int), SEEK_CUR);
        loc = ftell(fp_index);

        fseek(fp_record, loc / sizeof(int) * sizeof(*s2), SEEK_SET);
        fread(s2, sizeof(*s2), 1, fp_record);

        printf("\n\nStudent No :%d", s2->studentNo);
        printf("\n\nCourse Code :%d", s2->courseCode);
        printf("\n\nPoint :%d", s2->point);

        printf("\n\nDo You Want To Delete ? y/n  ");
        fflush(stdin);
        scanf("%c", &ch);

        if (ch == 'n' || ch == 'N')
            return;

        fseek(fp_index, sizeof(int), SEEK_CUR); //  delete index value
        while (fread(&r, sizeof(int), 1, fp_index))
        {
            fseek(fp_index, (-2) * sizeof(int), SEEK_CUR);
            fwrite(&r, sizeof(int), 1, fp_index);
            fseek(fp_index, sizeof(int), SEEK_CUR);
        }

        while (fread(s2, sizeof(*s2), 1, fp_record))
        {
            fseek(fp_record, (-2) * sizeof(*s2), SEEK_CUR);
            fwrite(s2, sizeof(*s2), 1, fp_record);
            fseek(fp_record, sizeof(*s2), SEEK_CUR);
        }
    }

    _chsize(file_handler_ind,file_size_ind-sizeof(int));
    _chsize(file_handler_rec,file_size_rec-sizeof(*s2));

    printf("\n\n");
    fclose(fp_index);
    fclose(fp_record);
}

void display(stud *s2)
{
    FILE *fp_data=NULL, *fp_index =NULL;
    int flag = 1, ch, r, size, num_elem=1;

    printf("\n\n1:Index File   2:Record File\n");
    printf("--------------------------------\n");
    printf("Make to Choice : ");
    scanf("%d", &ch);
    fp_index = fopen("index.txt", "rb");
    /*fseek(fp_index,sizeof(int), SEEK_END); // seek to end of file
    size = ftell(fp_index);
    fseek(fp_index,sizeof(int), SEEK_SET);
    printf("size: %d", size);
    num_elem =(size/sizeof(int))-1;
    printf("size: %d", num_elem);*/

    switch (ch)
    {
        case 1:

            while (fread(&r, sizeof(int), 1, fp_index))
            {
                flag = 0;

                printf("Index %d: ", num_elem);
                printf("%d\n", r);
                num_elem++;
            }

            break;

        case 2:

            fp_data = fopen("record.bin", "rb");

            while (fread(s2, sizeof(*s2), 1, fp_data))
            {
                if ((s2->studentNo) != 0)
                {

                    if (flag == 1)
                        printf("\nStudent No\t   Course Code\t   Point\n");
                        printf("---------------------------------------\n");
                    flag = 0;
                    printf("Rec-%d : ", num_elem);
                    printf("%d\t", s2->studentNo);
                    printf("\t%d\t", s2->courseCode);
                    printf("    %d\n", s2->point);
                    num_elem++;

                }
            }

            break;

        default:

            printf("\n\nInvalid choice !!!");
    }

    if (flag == 1)
        printf("\n\nNo record found !!!");
    printf("\n\n");
    fclose(fp_data);
    fclose(fp_index);
}


void binarySearchIndex(int sr, stud *s2, int low, int high)
{

    int r,r2, size, loc, flag = 0, mid;
    //stud s2;
    FILE *fp_index, *fp_record;
    fp_index = fopen("index.txt", "r+b");
    fp_record = fopen("record.bin", "r+b");
    //low = 0;
    //Middle Index
    //printf("%d", r);/

    //fseek(fp_index,sizeof(int), SEEK_END); // seek to end of file
    //size = ftell(fp_index); // get current file pointer
    //printf("Size: %d",size);
    //printf("Eleman Sayisi: %d", size/sizeof(int));
    //high = (size/sizeof(int))-1;
    //printf("High: %d",high);


    //while(low <= high)
    //{


        if (low>high)
        {
            return -1;
        }

        mid = low+(high-low)/2;
        fseek(fp_index, mid*4,SEEK_SET);
        fread(&r, sizeof(int), 1, fp_index);

        if(r == sr)
        {

            fseek(fp_index, (-1)*sizeof(int), SEEK_CUR);
            fread(&r2, sizeof(int), 1, fp_index);

            if(r2<r)
            {
                return mid;
            }

            else
            {
                fseek(fp_index, (-1)*sizeof(int), SEEK_CUR);
                loc = ftell(fp_index);
            //printf("Loc: \n%d",loc);
                fseek(fp_record, loc / sizeof(int) * sizeof(*s2), SEEK_SET);
                fread(s2, sizeof(*s2), 1, fp_record);

                printf("\n\nStudent No :%d", s2->studentNo);
                printf("\n\nCourse Code :%d", s2->courseCode);
                printf("\n\nPoint :%d\n\n", s2->point);


                binarySearchIndex(sr, s2, mid+1, high);
                binarySearchIndex(sr, s2, low, mid-1);

            }


        }

        else if(sr<r)
        {
            //high = mid-1;
            binarySearchIndex(sr, s2, low, mid-1);

        }

        else if(sr>r)
        {
            //low = mid+1;
           binarySearchIndex(sr, s2, mid+1, high);
        }
    //}

    fclose(fp_index);
    fclose(fp_record);
}


int main()
{
    FILE *fp_data, *fp_index;
    stud s1, s2;
    int choice, roll_no, binary_search_no, size, high;


    fp_data = fopen("record.bin", "a+b");
    fclose(fp_data);
    fp_index = fopen("index.txt", "a+b");
    fclose(fp_index);
    //program çalýþýr çalýþmaz dosyalar oluþturuldu

    while (1)
    {
        printf("1:Add   2:Display   3:Search   4:Edit   5:Delete   6:Binary Search   7:Delete Index File   8:Exit \n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("Make to Choice : ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                add(&s1);

                break;
            case 2:

                display(&s2);

                break;
            case 3:
                printf("\nEnter Student No To Search Record : ");
                scanf("%d", &roll_no);
                search(&s2, roll_no);

                break;
            case 4:
                printf("\n\nEnter Student No To Edit Record : ");
                scanf("%d", &roll_no);
                fp_index = fopen("index.txt", "a+b");
                fseek(fp_index, sizeof(int), SEEK_END); // seek to end of file
                size = ftell(fp_index);
                //printf("%d", size);
                //printf("Eleman Sayisi: %d", size/sizeof(int));
                high = (size/sizeof(int))-1;
                printf("Total Record: %d", high);
                edit(roll_no, &s2,0, high);
                fclose(fp_index);
                break;
            case 5:
                printf("\n\nEnter Student No To Delete Record : ");
                scanf("%d", &roll_no);

                delet(&s2, roll_no);

                break;
            case 6:
                printf("\n\nEnter Student No To Search Record : ");
                scanf("%d", &binary_search_no);
                fp_index = fopen("index.txt", "a+b");
                fseek(fp_index, sizeof(int), SEEK_END); // seek to end of file
                size = ftell(fp_index);
                //printf("%d", size);
                //printf("Eleman Sayisi: %d", size/sizeof(int));
                high = (size/sizeof(int))-1;
                printf("Total Record: %d", high);
                binarySearchIndex(binary_search_no, &s2,0, high);
                fclose(fp_index);
                break;

            case 7:
                remove("index.txt");
                printf("\n\n******Index File Deleted******\n\n\n");
                break;

            case 8:
                exit(0);

            default:
                printf("\n\nInvalid choice !!!");
        }
    }

    return 0;
}
