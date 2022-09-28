
#ifndef RectangleSelectionTool_H
#define RectangleSelectionTool_H

struct RectangleSelectionTool {
    int xStart , yStart;
    int xEnd , yEnd;
    bool isAdding; // or it is removing
    bool isActive;
    RectangleSelectionTool() : xStart(0) , xEnd(0) , yStart(0) , yEnd(0) , isActive(false) {}
    void initRectangle(int x , int y) {
        xStart = xEnd = x;
        yStart = yEnd = y;
    }
    void updateRectangle(int x , int y) {
        xEnd = x;
        yEnd = y;
    }

    bool contains(float xx , float yy) const {
        float viewport[4]; glGetFloatv( GL_VIEWPORT , viewport );
        float w = viewport[2] , h = viewport[3];
        float left = (float)(min<int>(xStart,xEnd)) / w;
        float right = (float)(max<int>(xStart,xEnd)) / w;
        float top = 1.f - (float)(min<int>(yStart,yEnd)) / h;
        float bottom = 1.f - (float)(max<int>(yStart,yEnd)) / h;

        return (left <= xx) && (xx <= right) && (bottom <= yy) && (yy <= top);
    }

    void draw() {
        if(! isActive) return;

        float viewport[4]; glGetFloatv( GL_VIEWPORT , viewport );
        float w = viewport[2] , h = viewport[3];
        float left = (float)(min<int>(xStart,xEnd)) / w;
        float right = (float)(max<int>(xStart,xEnd)) / w;
        float top = 1.f - (float)(min<int>(yStart,yEnd)) / h;
        float bottom = 1.f - (float)(max<int>(yStart,yEnd)) / h;

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);

        glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );

        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        glLoadIdentity();
        glOrtho( 0.f , 1.f , 0.f , 1.f , -1.f , 1.f );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();

        glColor4f(1, 1, 1 , 0.4f); // transparent white in the middle

        glBegin(GL_QUADS);
        glVertex2f( left , top );
        glVertex2f( left , bottom );
        glVertex2f( right , bottom );
        glVertex2f( right , top );
        glEnd();

        glLineWidth(2.0);

        if(isAdding)
            glColor4f(0.1, 0.1, 1.f , 0.5f); // adding -> blue
        else
            glColor4f(1.0, 0.1, 0.1 , 0.5f); // removing -> red

        glBegin(GL_LINE_LOOP);
        glVertex2f( left , top );
        glVertex2f( left , bottom );
        glVertex2f( right , bottom );
        glVertex2f( right , top );
        glEnd();

        glPopMatrix();
        glMatrixMode( GL_PROJECTION );
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
};

#endif // RectangleSelectionTool_H
