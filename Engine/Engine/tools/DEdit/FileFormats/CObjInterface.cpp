// CObjInterface.cpp : implementation file
//

#include "bdefs.h"
#include "ObjectImporter.h"
#include "DEdit.h"
#include "RegionDoc.h"
#include "RegionView.h"
#include "RegionFrame.h"
#include "UsefulDC.h"
#include "EditPoly.h"
#include "StringDlg.h"
#include "EditHelpers.h"
#include "AddObjectDlg.h"
#include "PropertiesDlg.h"
#include "WorldInfoDlg.h"
#include "resource.h"
#include "ClassDlg.h"
#include "MainFrm.h"
#include "MapTextureCoordsDlg.h"
#include "ProjectBar.h"
#include "geomroutines.h"
#include "RotateSelection.h"
#include "Texture.h"
#include "edit_actions.h"
#include "de_world.h"
#include "resource.h"
#include "node_ops.h"
#include "ColorPicker.h"
#include "navigatordlg.h"
#include "NavigatorStoreDlg.h"
#include "AdvancedSelect.h"
#include "AdvancedSelectDlg.h"
#include "ObjectBrowserDlg.h"
#include "OptionsObjectBrowser.h"
#include "VectorEdit.h"
#include "GenerateUniqueNamesDlg.h"
#include "NameChangeReportDlg.h"
#include "OptionsGenerateUniqueNames.h"

#include "CObjInterface.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/************************************************************************/
void CObjInterface::FileGetLine(char* line, int n, FILE* file)
{
	assert(line && n && file);
	if(line && n && file)
	{
		char ch; 
		int i;

		
		for( i=0; (i < n ) && ( feof( file ) == 0 ); i++ )
		{
			ch = fgetc( file );
			line[i] = (char)ch;
			// ch = fgetc( file );
			if(ch == '\r')
			  break;
			else if(ch == '\n')
			  break;
		}
		line[i] = '\0';
	}
}
/************************************************************************/
int CObjInterface::FileSize( FILE* file )
{
	int i = 0;
	assert(file);
	if( file )
	{
		rewind(file);
		while(  feof( file ) == 0 )
		{
			char ch;
			ch = fgetc( file );
			i++;
		}
		rewind(file);
	}
	return i;
}
/************************************************************************/
void CObjInterface::FileCopyToBuffer(FILE* file, char* buffer)
{
	assert(file);
	if( file )
	{
		int i = 0;
		rewind(file);
		while(  feof( file ) == 0 )
		{
		   buffer[i] = fgetc( file );
		   i++;
		}
		rewind(file);
	}
}
/************************************************************************/
void CObjInterface::ImportObj(FILE* file, int flag, int from)
{
	assert(file);
	if( file ) 
	{
		m_flag = flag;
		m_from = from;
		// read in all the verts			token(v)
		ImportObjVertices( file );
 		// read in all the texture coords	token(vt)
		ImportObjTexCoords( file );
		// read in all the normals			token(vn)
//		ImportObjNormals( file );
		// read in all the objects			token(g,f)
		ImportObjObjects( file );
	}
}
/************************************************************************/
#define OBJ_MAX_BUFFER_SIZE 256
void CObjInterface::ImportObjVertices( FILE* file )
{
	assert(file);
	if( file )
	{
		char buffer[OBJ_MAX_BUFFER_SIZE+1];
		rewind(file);
		while(feof( file ) == 0)
		{
			FileGetLine(buffer, OBJ_MAX_BUFFER_SIZE, file);
			if( strlen(buffer) > 2 )
			{
				if( buffer[0] == 'v'  && buffer[1] == ' ' )
				{
					char seps[] = " \t\n\r";	
					char *token = NULL;	
					int   i = 0;
					float v[3];

					token = strtok( buffer, seps );
					if(token != NULL)
					{
						token = strtok( NULL, seps );  // let's eat the leading token
						i = 0;
						while( (token != NULL) && (i < 3) )
						{
							v[i++] = atof(token);
							token = strtok( NULL, seps ); // Get next token	
						}
						switch(m_from)
						{
						case F3DSMAX:
						case FMAYA:
						case FSOFT:
							{
								LTVector vert(v[0],v[2],v[1]); 
								m_vertexList.Append(vert);
							}
							break;
						default:
							{
								LTVector vert(v[0],v[1],v[2]); 
								m_vertexList.Append(vert);
							}
							break;
						}
					}
				}
			}
				
		}
	}
}
/************************************************************************/
void CObjInterface::ImportObjTexCoords( FILE* file )
{
	assert(file);
	if( file )
	{
		char buffer[OBJ_MAX_BUFFER_SIZE+1];
		rewind(file);
		while(feof( file ) == 0)
		{
			FileGetLine(buffer, OBJ_MAX_BUFFER_SIZE, file);
			if( strlen(buffer) > 2 )
			{
				if( buffer[0] == 'v'  && buffer[1] == 't' )
				{
					char seps[] = " \t\n\r";	
					char *token = NULL;	
					int   i = 0;
					float v[3];

					token = strtok( buffer, seps );
					if(token != NULL)
					{
						token = strtok( NULL, seps );  // let's eat the leading token
						i = 0;
						while( (token != NULL) && (i < 3) )
						{
							v[i++] = atof(token);
							token = strtok( NULL, seps ); // Get next token	
						}
						LTVector vert(v[0],v[1],v[2]); 
						m_texCoordList.Append(vert);
					}
				}
			}
				
		}
	}
}
/************************************************************************/
void CObjInterface::ImportObjNormals( FILE* file )
{
	assert(file);
	if( file )
	{
		char buffer[OBJ_MAX_BUFFER_SIZE+1];
		rewind(file);
		while(feof( file ) == 0)
		{
			FileGetLine(buffer, OBJ_MAX_BUFFER_SIZE, file);
			if( strlen(buffer) > 2 )
			{
				if( buffer[0] == 'v'  && buffer[1] == 'n' )
				{
					char seps[] = " \t\n\r";	
					char *token = NULL;	
					int   i = 0;
					float v[3];

					token = strtok( buffer, seps );
					if(token != NULL)
					{
						token = strtok( NULL, seps );  // let's eat the leading token
						i = 0;
						while( (token != NULL) && (i < 3) )
						{
							v[i++] = atof(token);
							token = strtok( NULL, seps ); // Get next token	
						}
						//! gmVector3 vert(v[0],v[1],v[2]); 
						//! m_normalList.push_back(vert);		
					}
				}
			}
				
		}
	}
}
/************************************************************************/
void CObjInterface::ImportObjObjects( FILE* file )
{
	assert(file);
	if( file )
	{
		char buffer[OBJ_MAX_BUFFER_SIZE+1];
		rewind(file);
		while(feof( file ) == 0)
		{
			FileGetLine(buffer, OBJ_MAX_BUFFER_SIZE, file);
			if( strlen(buffer) > 2 )
			{
				if( buffer[0] == 'f'  && buffer[1] == ' ' )
				{
					char seps[] = "f /\t\n\r";	
					char *token = NULL;	
					int   i = 0;
					CMoArray< int > intList;

					token = strtok( buffer, seps );
					if(token != NULL)
					{
						// found a face
						while( (token != NULL) )
						{
							intList.Append(atoi(token));
							token = strtok( NULL, seps ); // Get next token	
						}
						m_faceSetList.Append(intList);
					}
				}
			}
				
		}
	}
}
/************************************************************************/
void CObjInterface::ExportObj(FILE* file, int flag)
{
	assert(file);
	if(file)
	{
		m_flag = flag;
		int i = 0;
		fprintf(file, "\ng default");
		for( i = 0; i < m_vertexList.GetSize(); i++ )
		{
			fprintf(file, "\nv %f %f %f",m_vertexList[i][0],m_vertexList[i][1],m_vertexList[i][2]);
		}
		fprintf(file, "\ng object");
		for( int j = 0; j < m_faceSetList.GetSize(); j++ )
		{
			if(m_faceSetList[j].GetSize())
			{
				fprintf(file, "\nf " );
				if( m_flag == CObjInterface::RAW )
				{
					for( int k = 0; k < m_faceSetList[j].GetSize(); k++ )
					{
						fprintf(file, "%d ",m_faceSetList[j][k]);
					}
				}
				else
				{
					for( int k = m_faceSetList[j].GetSize()-1; k >= 0 ; k-- )
					{
						fprintf(file, "%d ",m_faceSetList[j][k]);
					}
				}
			}
		}	
	}
}


